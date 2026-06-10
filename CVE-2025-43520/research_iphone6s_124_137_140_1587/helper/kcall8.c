#include "offsets.h"
#include "tfp0_krw.h"
#include "kexecute.h"

extern bool g_Is_iPad7_150;
extern bool g_Is_iPad7_137;
extern bool g_Is_iPhone6s_140;

extern uint64_t gKernelSlide;


// for iOS 15.0/iPad7,11
uint64_t tfp0_kcall8(uint64_t addr, uint64_t x0, uint64_t x1, uint64_t x2, uint64_t x3, uint64_t x4, uint64_t x5, uint64_t x6, uint64_t x7) {
#if 0
    if(addr != ksym(KSYMBOL_ml_phys_read_data) && addr != ksym(KSYMBOL_MMU_KVTOP)) 
        printf("tfp0_kcall8 addr: 0x%llx, addr(nokslide): 0x%llx\n", addr, addr-gKernelSlide);
#endif
    uint64_t kret = 0;

    if(g_Is_iPad7_137 == true) {
        // for iOS 13.7/iPad7,11
        #define BLK1   0x000
        #define BLK2   0x100
        #define BLK3   0x180
        #define BLK4   0x200
        #define BLK5   0x280
        #define BLK5_5 0x300
        #define BLK6   0x380
        #define STORED_RET_OFF 0x400
        uint64_t kpage = tfp0_kalloc(0x1000);

        tfp0_kwrite64(kpage, kpage);
        tfp0_kwrite64(kpage + 0x98, kpage);
        tfp0_kwrite64(kpage + 0x7C0, kgad(KGADGET_POPULATE));
    
        tfp0_kwrite64(kpage + BLK1 + 0x10, kgad(KGADGET_MOV_X10_X1__BR_X2));
        tfp0_kwrite64(kpage + BLK1 + 0x18, kpage + BLK2);
        tfp0_kwrite64(kpage + BLK1 + 0x20, kgad(KGADGET_POPULATE));
        tfp0_kwrite64(kpage + BLK1 + 0x28, kgad(KGADGET_POPULATE));
    
        tfp0_kwrite64(kpage + BLK2 + 0x10, kgad(KGADGET_MOV_X20_X3__BR_X2));
        tfp0_kwrite64(kpage + BLK2 + 0x18, kpage + BLK3);
        tfp0_kwrite64(kpage + BLK2 + 0x28, kgad(KGADGET_POPULATE));     
        tfp0_kwrite64(kpage + BLK2 + 0x30, x0);                     
    
        tfp0_kwrite64(kpage + BLK3 + 0x10, kgad(KGADGET_MOV_X13_X1__BR_X2));
        tfp0_kwrite64(kpage + BLK3 + 0x18, kpage + BLK4);
        tfp0_kwrite64(kpage + BLK3 + 0x20, addr);                      
        tfp0_kwrite64(kpage + BLK3 + 0x28, kgad(KGADGET_POPULATE));   
    
        tfp0_kwrite64(kpage + BLK4 + 0x10, kgad(KGADGET_MOV_X11_X13__BR_X10));
        tfp0_kwrite64(kpage + BLK4 + 0x18, kpage + BLK5);
    
        tfp0_kwrite64(kpage + BLK5 + 0x10, kgad(KGADGET_MOV_X10_X1__BR_X2));
        tfp0_kwrite64(kpage + BLK5 + 0x18, kpage + BLK5_5);
        tfp0_kwrite64(kpage + BLK5 + 0x20, kgad(KGADGET_MOV_X0_X20__BR_X11)); 
        tfp0_kwrite64(kpage + BLK5 + 0x28, kgad(KGADGET_POPULATE));
    
        tfp0_kwrite64(kpage + BLK5_5 + 0x10, kgad(KGADGET_MOV_X7_X1__BR_X8));
        tfp0_kwrite64(kpage + BLK5_5 + 0x18, kgad(KGADGET_MOV_X0_X3__BR_X4));
        tfp0_kwrite64(kpage + BLK5_5 + 0x20, x7);                              
        tfp0_kwrite64(kpage + BLK5_5 + 0x30, kpage + BLK6);                    
        tfp0_kwrite64(kpage + BLK5_5 + 0x38, kgad(KGADGET_POPULATE));        
    
        tfp0_kwrite64(kpage + BLK6 + 0x10, kgad(KGADGET_MOV_X5_X8__BR_X10));
        tfp0_kwrite64(kpage + BLK6 + 0x18, x5);
        tfp0_kwrite64(kpage + BLK6 + 0x20, x1);
        tfp0_kwrite64(kpage + BLK6 + 0x28, x2);
        tfp0_kwrite64(kpage + BLK6 + 0x30, x3);
        tfp0_kwrite64(kpage + BLK6 + 0x38, x4);                          

        uint64_t STORED_RET = kpage + STORED_RET_OFF;

        kexecute(kgad(KGADGET_PROLOGUE), kpage, STORED_RET, 0, 0, 0, 0, x6);

        kret =  tfp0_kread64(STORED_RET);
        tfp0_kfree(kpage, 0x1000);
    }
    else if(g_Is_iPhone6s_140 == true) {
        #define BLK1   0x000 
        #define BLK2   0x100
        #define BLK3   0x180
        #define BLK4   0x200
        #define BLK5   0x280
        #define BLK6   0x300
        #define BLK7   0x380
        #define BLK8   0x400
        #define BLK9   0x480 
        #define STORED_RET_OFF 0x500
        uint64_t kpage = tfp0_kalloc(0x1000);

        tfp0_kwrite64(kpage,         kpage);
        tfp0_kwrite64(kpage + 0x98,  kpage);
        tfp0_kwrite64(kpage + 0x7C0, kgad(KGADGET_POPULATE));

        tfp0_kwrite64(kpage + BLK1 + 0x10, kgad(KGADGET_MOV_X12_X0__BR_X2));
        tfp0_kwrite64(kpage + BLK1 + 0x18, kgad(KGADGET_POPULATE));
        tfp0_kwrite64(kpage + BLK1 + 0x28, kgad(KGADGET_MOV_X0_X3__BR_X4));
        tfp0_kwrite64(kpage + BLK1 + 0x30, kpage + BLK2);
        tfp0_kwrite64(kpage + BLK1 + 0x38, kgad(KGADGET_POPULATE));

        tfp0_kwrite64(kpage + BLK2 + 0x10, kgad(KGADGET_MOV_X15_X2__BR_X3));
        tfp0_kwrite64(kpage + BLK2 + 0x18, kpage + BLK3);
        tfp0_kwrite64(kpage + BLK2 + 0x28, x0);
        tfp0_kwrite64(kpage + BLK2 + 0x30, kgad(KGADGET_POPULATE));

        tfp0_kwrite64(kpage + BLK3 + 0x10, kgad(KGADGET_MOV_X20_X15__BR_X12));
        tfp0_kwrite64(kpage + BLK3 + 0x18, kpage + BLK4);

        tfp0_kwrite64(kpage + BLK4 + 0x10, kgad(KGADGET_MOV_X11_X1__BR_X12));
        tfp0_kwrite64(kpage + BLK4 + 0x18, kpage + BLK5);
        tfp0_kwrite64(kpage + BLK4 + 0x20, addr);

        tfp0_kwrite64(kpage + BLK5 + 0x10, kgad(KGADGET_MOV_X16_X1__BR_X2));
        tfp0_kwrite64(kpage + BLK5 + 0x18, kpage + BLK6);
        tfp0_kwrite64(kpage + BLK5 + 0x20, x7);
        tfp0_kwrite64(kpage + BLK5 + 0x28, kgad(KGADGET_POPULATE));

        tfp0_kwrite64(kpage + BLK6 + 0x10, kgad(KGADGET_MOV_X10_X12__BR_X8));
        tfp0_kwrite64(kpage + BLK6 + 0x18, kgad(KGADGET_MOV_X0_X1__BR_X2));
        tfp0_kwrite64(kpage + BLK6 + 0x20, kpage + BLK7);
        tfp0_kwrite64(kpage + BLK6 + 0x28, kgad(KGADGET_POPULATE));

        tfp0_kwrite64(kpage + BLK7 + 0x10, kgad(KGADGET_MOV_X7_X16__BR_X10));
        tfp0_kwrite64(kpage + BLK7 + 0x18, kpage + BLK8);

        tfp0_kwrite64(kpage + BLK8 + 0x10, kgad(KGADGET_MOV_X10_X0__BR_X2));
        tfp0_kwrite64(kpage + BLK8 + 0x18, kgad(KGADGET_MOV_X0_X20__BR_X11));
        tfp0_kwrite64(kpage + BLK8 + 0x28, kgad(KGADGET_MOV_X0_X3__BR_X4));
        tfp0_kwrite64(kpage + BLK8 + 0x30, kpage + BLK9);
        tfp0_kwrite64(kpage + BLK8 + 0x38, kgad(KGADGET_POPULATE));

        tfp0_kwrite64(kpage + BLK9 + 0x10, kgad(KGADGET_MOV_X5_X8__BR_X10));
        tfp0_kwrite64(kpage + BLK9 + 0x18, x5);
        tfp0_kwrite64(kpage + BLK9 + 0x20, x1);
        tfp0_kwrite64(kpage + BLK9 + 0x28, x2);
        tfp0_kwrite64(kpage + BLK9 + 0x30, x3);
        tfp0_kwrite64(kpage + BLK9 + 0x38, x4);
        uint64_t STORED_RET = kpage + STORED_RET_OFF;

        kexecute(kgad(KGADGET_PROLOGUE), kpage, STORED_RET, 0, 0, 0, 0, x6);

        kret =  tfp0_kread64(STORED_RET);
        tfp0_kfree(kpage, 0x1000);
    }
    else if(g_Is_iPad7_150 == true) {
        uint64_t kpage = tfp0_kalloc(0x1000);

        tfp0_kwrite64(kpage, kpage);
        tfp0_kwrite64(kpage + 0x98, kpage);
        tfp0_kwrite64(kpage + 0x7C0, kgad(KGADGET_POPULATE));
        tfp0_kwrite64(kpage + 0x10, kgad(KGADGET_MOV_X14_X3__BR_X4));
        tfp0_kwrite64(kpage + 0x18, kpage + 0x800);
        tfp0_kwrite64(kpage + 0x20, kgad(KGADGET_POPULATE));
        tfp0_kwrite64(kpage + 0x28, kgad(KGADGET_MOV_X7_X14__BR_X15));
        tfp0_kwrite64(kpage + 0x30, x7);
        tfp0_kwrite64(kpage + 0x38, kgad(KGADGET_MOV_X15_X1__BR_X2));
        tfp0_kwrite64(kpage + 0x810, kgad(KGADGET_MOV_X10_X1__BR_X3));
        tfp0_kwrite64(kpage + 0x818, kpage + 0x840);
        tfp0_kwrite64(kpage + 0x820, kgad(KGADGET_POPULATE));
        tfp0_kwrite64(kpage + 0x828, kgad(KGADGET_POPULATE));
        tfp0_kwrite64(kpage + 0x830, kgad(KGADGET_MOV_X11_X4__BR_X2));
        tfp0_kwrite64(kpage + 0x838, addr);
        tfp0_kwrite64(kpage + 0x850, kgad(KGADGET_MOV_X15_X2__BR_X3));
        tfp0_kwrite64(kpage + 0x858, kpage + 0x880);
        tfp0_kwrite64(kpage + 0x860, x6);
        tfp0_kwrite64(kpage + 0x868, kgad(KGADGET_MOV_X6_X9__BR_X11));
        tfp0_kwrite64(kpage + 0x870, kgad(KGADGET_MOV_X9_X1__BR_X10));
        tfp0_kwrite64(kpage + 0x878, 0);
        tfp0_kwrite64(kpage + 0x890, kgad(KGADGET_MOV_X5_X6__BR_X15));
        tfp0_kwrite64(kpage + 0x898, x0);
        tfp0_kwrite64(kpage + 0x8A0, x1);
        tfp0_kwrite64(kpage + 0x8A8, x2);
        tfp0_kwrite64(kpage + 0x8B0, x3);
        tfp0_kwrite64(kpage + 0x8B8, x4);
        x6 = x5;

        uint64_t STORED_RET = kpage + 0x100;
        kexecute(kgad(KGADGET_PROLOGUE), kpage, STORED_RET, 0, 0, 0, x5, x6);

        kret =  tfp0_kread64(STORED_RET);
        tfp0_kfree(kpage, 0x1000);
    }
    else {
        printf("[%s:%d] kcall8 unsupported; Spinning here...\n", __FUNCTION__, __LINE__);
        while(1) {};
    }

    return kret;
}

uint64_t kvtophys(uint64_t va)
{
	/**
	 * Attempt to do the translation first in hardware using the AT (address
	 * translation) instruction. This will attempt to use the MMU to do the
	 * translation for us.
	 */
	// uint64_t pa = mmu_kvtop(va);
    uint64_t pa = tfp0_kcall8(ksym(KSYMBOL_MMU_KVTOP), va, 0, 0, 0, 0, 0, 0, 0);

	if (pa) {
		return pa;
	}

	/* If the MMU can't find the mapping, then manually walk the page tables. */
	// return pmap_vtophys(kernel_pmap, va);
    return tfp0_kcall8(ksym(KSYMBOL_PMAP_VTOPHYS), ksym(KSYMBOL_KERNEL_PMAP), va, 0, 0, 0, 0, 0, 0);
}


#if 0 
uint64_t phystokv(uint64_t pa) {
    return tfp0_kcall8(ksym(KSYMBOL_PHYSTOKV), pa, 0, 0, 0, 0, 0, 0, 0);
}
#endif 

uint64_t physread64(uint64_t pa) {
    union {
        uint32_t u32[2];
        uint64_t u64;
    } u;

    u.u32[0] = (uint32_t)tfp0_kcall8(ksym(KSYMBOL_ml_phys_read_data), pa, 4, 0, 0, 0, 0, 0, 0);
    u.u32[1] = (uint32_t)tfp0_kcall8(ksym(KSYMBOL_ml_phys_read_data), pa+4, 4, 0, 0, 0, 0, 0, 0);
    return u.u64;
}

int physreadbuf(uint64_t pa, void *buf, size_t sz)
{
    if (buf == NULL || sz == 0) { return 0; }

    uint8_t *out       = (uint8_t *)buf;
    size_t   remaining = sz;

    while (remaining > 0) {
        uint64_t aligned_pa = pa & ~7ULL;
        uint64_t offset     = pa - aligned_pa;  
        size_t   chunk      = 8 - offset;        
        if (chunk > remaining) chunk = remaining;

        uint64_t val = physread64(aligned_pa);
        memcpy(out, (uint8_t *)&val + offset, chunk);

        out       += chunk;
        pa        += chunk;
        remaining -= chunk;
    }
    return 0;
}

uint64_t pmap_vtophys(uint64_t pmap, uint64_t va) {
    return tfp0_kcall8(ksym(KSYMBOL_PMAP_VTOPHYS), pmap, va, 0, 0, 0, 0, 0, 0);
}

uint64_t pmap_find_pa(uint64_t pmap, uint64_t va) {
    // Since pmap_find_pa is NOT exists on iOS 13.7, use pmap_vtophys instead.
    // uint64_t kret = tfp0_kcall8(ksym(KSYMBOL_pmap_find_pa), pmap, va, 0, 0, 0, 0, 0, 0);
    // return kret;
    return pmap_vtophys(pmap, va);
}

void phexdump(uint64_t addr, size_t size) {
    void *data = malloc(size);
    physreadbuf(addr, data, size);
    char ascii[17];
    size_t i, j;
    ascii[16] = '\0';
    for (i = 0; i < size; ++i) {
        if ((i % 16) == 0)
        {
            printf("[0x%016llx+0x%03zx] ", addr, i);
//            printf("[0x%016llx] ", i + addr);
        }
        
        printf("%02X ", ((unsigned char*)data)[i]);
        if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
            ascii[i % 16] = ((unsigned char*)data)[i];
        } else {
            ascii[i % 16] = '.';
        }
        if ((i+1) % 8 == 0 || i+1 == size) {
            printf(" ");
            if ((i+1) % 16 == 0) {
                printf("|  %s \n", ascii);
            } else if (i+1 == size) {
                ascii[(i+1) % 16] = '\0';
                if ((i+1) % 16 <= 8) {
                    printf(" ");
                }
                for (j = (i+1) % 16; j < 16; ++j) {
                    printf("   ");
                }
                printf("|  %s \n", ascii);
            }
        }
    }
    free(data);
}