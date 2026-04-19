#include "tfp0_krw.h"
#include "offsets.h"
#include "pmap.h"
// #include "physrw.h"
#include "translation.h"
// #include "kfunc.h"
#include "pte.h"
#include "pvh.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

vm_size_t get_kernel_page_size() {
    vm_size_t kernel_page_size = 0;
    vm_size_t *out_page_size = NULL;
    host_t host = mach_host_self();
    if (!MACH_PORT_VALID(host)) goto out;
    out_page_size = (vm_size_t *)malloc(sizeof(vm_size_t));
    if (out_page_size == NULL) goto out;
    bzero(out_page_size, sizeof(vm_size_t));
    if (_host_page_size(host, out_page_size) != KERN_SUCCESS) goto out;
    kernel_page_size = *out_page_size;
out:
    if (MACH_PORT_VALID(host)) mach_port_deallocate(mach_task_self(), host); host = HOST_NULL;
    if(out_page_size != NULL) free(out_page_size);
    return kernel_page_size;
}

uint64_t get_l1_block_size(void)
{
	switch (vm_real_kernel_page_size) {
		case 0x4000:
		    return 0x1000000000;
		case 0x1000:
		    return 0x40000000;
		default:
		    return 0;
	}
}

uint64_t get_l1_block_mask(void)
{
	return get_l1_block_size() - 1;
}

uint64_t get_l1_block_count(void)
{
	switch (vm_real_kernel_page_size) {
		case 0x4000:
		    return 8;
		case 0x1000:
		    return 256;
		default:
		    return 0;
	}
}

uint64_t get_l2_block_size(void)
{
	switch (vm_real_kernel_page_size) {
		case 0x4000:
		    return 0x2000000;
		case 0x1000:
		    return 0x200000;
		default:
		    return 0;
	}
}

uint64_t get_l2_block_mask(void)
{
	return get_l2_block_size() - 1;
}

uint64_t get_l2_block_count(void)
{
	switch (vm_real_kernel_page_size) {
		case 0x4000:
		    return 2048;
		case 0x1000:
		    return 512;
		default:
		    return 0;
	}
}

#if 0 
int pmap_expand_range(uint64_t pmap, uint64_t vaStart, uint64_t size)
{
	uint64_t ttep = tfp0_kread64(pmap + off_pmap_ttep);

    uint64_t unmappedStart = 0, unmappedSize = 0;

	uint64_t l2Start = vaStart & ~L2_BLOCK_MASK;
	uint64_t l2End = (vaStart + (size - 1)) & ~L2_BLOCK_MASK;
	uint64_t l2Count = ((l2End - l2Start) / L2_BLOCK_SIZE) + 1;

	for (uint64_t i = 0; i <= l2Count; i++) {
		uint64_t curL2 = l2Start + (i * L2_BLOCK_SIZE);

		uint64_t leafLevel = PMAP_TT_L3_LEVEL;
		uint64_t pt3 = 0;
		vtophys_lvl(ttep, curL2, &leafLevel, &pt3);
		if (leafLevel == PMAP_TT_L3_LEVEL || i == l2Count) {
			// i == l2Count: one extra cycle that this for loop takes
			// We hit this block either if there was a mapping or at the end
			// Alloc page tables for the current area (unmappedStart, unmappedSize) by running pmap_enter_options on every page
			// And then running pmap_remove on the entire area while nested is true

			for (uint64_t l2Off = 0; l2Off < unmappedSize; l2Off += L2_BLOCK_SIZE) {
				kern_return_t kr = kfunc_pmap_enter_options_addr(pmap, FAKE_PHYSPAGE_TO_MAP, unmappedStart + l2Off);
				if (kr != KERN_SUCCESS) {
					return -7;
				}
			}

			// Set type to nested
			physwrite8(kvtophys(pmap + off_pmap_type), 3);

			// Remove mapping (table will stay cause nested is set)
			pmap_remove(pmap, unmappedStart, unmappedStart + unmappedSize);

			// Change type back
			physwrite8(kvtophys(pmap + off_pmap_type), 0);
			
			unmappedStart = 0;
			unmappedSize = 0;
			continue;
		}
		else {
			if (unmappedStart == 0) {
				unmappedStart = curL2;
			}
			unmappedSize += L2_BLOCK_SIZE;
		}
	}

    return 0;
}

void pmap_remove(uint64_t pmap, uint64_t start, uint64_t end)
{
    uint64_t remove_count = 0;
    if (!pmap) {
        return;
    }
    uint64_t va = start;
    while (va < end) {
        uint64_t l;
        l = ((va + L2_BLOCK_SIZE) & ~L2_BLOCK_MASK);
        if (l > end) {
            l = end;
        }
        remove_count = kfunc_pmap_remove_options(pmap, va, l);
        va = remove_count;
    }
}

int pmap_map_in(uint64_t pmap, uint64_t uaStart, uint64_t paStart, uint64_t size)
{
	uint64_t ttep = tfp0_kread64(pmap + off_pmap_ttep);
    // printf("ttep = 0x%llx\n", ttep);

	uint64_t paEnd = paStart + size;
	uint64_t uaEnd = uaStart + size;

	uint64_t uaL2Start = uaStart & ~L2_BLOCK_MASK;
	uint64_t uaL2End   = ((uaStart + size - 1) + L2_BLOCK_SIZE) & ~L2_BLOCK_MASK;

	uint64_t paL2Start = paStart & ~L2_BLOCK_MASK;
	uint64_t l2Count = (((uaL2End - uaL2Start) - 1) / L2_BLOCK_SIZE) + 1;

	// Sanity check: Ensure the entire area to be mapped in is not mapped to anything yet
	for(uint64_t ua = uaStart; ua < uaEnd; ua += vm_real_kernel_page_size) {
		uint64_t leafLevel = PMAP_TT_L3_LEVEL;
		if (vtophys_lvl(ttep, ua, &leafLevel, NULL) != 0) {
			return -1;
		}
		else {
			// Performance improvement
			// If there is no L1 / L2 mapping we can skip a whole bunch of addresses
			if (leafLevel == PMAP_TT_L1_LEVEL) {
				ua = (((ua + L1_BLOCK_SIZE) & ~L1_BLOCK_MASK) - vm_real_kernel_page_size);
			}
			else if (leafLevel == PMAP_TT_L2_LEVEL) {
				ua = (((ua + L2_BLOCK_SIZE) & ~L2_BLOCK_MASK) - vm_real_kernel_page_size);
			}
		}

		if (vtophys(ttep, ua)) return -1;
		// TODO: If all mappings match 1:1, maybe return 0 instead of -1?
	}

	// Allocate all page tables that need to be allocated
	if (pmap_expand_range(pmap, uaStart, size) != 0) return -1;
	
	// Insert entries into L3 pages
	for (uint64_t i = 0; i < l2Count; i++) {
		uint64_t uaL2Cur = uaL2Start + (i * L2_BLOCK_SIZE);
		uint64_t paL2Cur = paL2Start + (i * L2_BLOCK_SIZE);

		// Create full table for this mapping
		uint64_t tableToWrite[L2_BLOCK_COUNT];
		for (int k = 0; k < L2_BLOCK_COUNT; k++) {
			uint64_t curMappingPage = paL2Cur + (k * vm_real_kernel_page_size);
			if (curMappingPage >= paStart && curMappingPage < paEnd) {
				tableToWrite[k] = curMappingPage | PERM_TO_PTE(PERM_KRW_URW) | PTE_NON_GLOBAL | PTE_OUTER_SHAREABLE | PTE_LEVEL3_ENTRY;
			}
			else {
				tableToWrite[k] = 0;
			}
		}

		// Replace table with the entries we generated
		uint64_t leafLevel = PMAP_TT_L2_LEVEL;
		uint64_t level2Table = vtophys_lvl(ttep, uaL2Cur, &leafLevel, NULL);
		if (!level2Table) return -2;
		physwritebuf(level2Table, tableToWrite, vm_real_kernel_page_size);

        // Reference count of new page table must be 0!
	    // original ref count is 1 because the table holds one PTE
	    // Our new PTEs are not part of the pmap layer though so refcount needs to be 0
        uint64_t pvh = pai_to_pvh(pa_index(level2Table));
		uint64_t ptdp = pvh_ptd(pvh);
        // printf("ptdp = 0x%llx, off_pt_desc_ptd_info = 0x%x\n", ptdp, off_pt_desc_ptd_info);

		uint16_t pinfo_refCount = kread16(ptdp + off_pt_desc_ptd_info);
        // printf("pinfo_refCount = 0x%hx\n", pinfo_refCount);

        kwrite16(ptdp + off_pt_desc_ptd_info, 0);
	}

	return 0;
}

#define atop(x) ((vm_address_t)(x) >> vm_kernel_page_shift)
uint64_t pa_index(uint64_t pa)
{
	return atop(pa - tfp0_kread64(ksym(KSYMBOL_vm_first_phys)));
}

uint64_t pai_to_pvh(uint64_t pai)
{
	return tfp0_kread64(ksym(KSYMBOL_pv_head_table)) + (pai * 8);
}

uint64_t pvh_ptd(uint64_t pvh)
{
	return ((tfp0_kread64(pvh) & PVH_LIST_MASK) | PVH_HIGH_FLAGS);
}
#endif