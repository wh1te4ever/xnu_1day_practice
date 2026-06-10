#include <stdint.h>

#define T8010_19A346_KVERSION "Darwin Kernel Version 21.0.0: Sun Aug 15 20:55:57 PDT 2021; root:xnu-8019.12.5~1/RELEASE_ARM64_T8010"
#define T8010_17H35_KVERSION "Darwin Kernel Version 19.6.0: Sat Jul 11 00:58:54 PDT 2020; root:xnu-6153.142.1~8/RELEASE_ARM64_T8010"
#define S8000_18A373_KVERSION "Darwin Kernel Version 20.0.0: Fri Aug 28 23:05:58 PDT 2020; root:xnu-7195.0.46~9/RELEASE_ARM64_S8000"

enum kgadget {
  KGADGET_POPULATE,
  KGADGET_PROLOGUE,
  KGADGET_ADD_X0_X0_0X40__RET,
  KGADGET_MOV_X5_X6__BR_X15,
  KGADGET_MOV_X6_X9__BR_X11,
  KGADGET_MOV_X7_X14__BR_X15,
  KGADGET_MOV_X9_X1__BR_X10,
  KGADGET_MOV_X15_X2__BR_X3,
  KGADGET_MOV_X15_X1__BR_X2,
  KGADGET_MOV_X14_X3__BR_X4,
  KGADGET_MOV_X11_X4__BR_X2,
  KGADGET_MOV_X10_X1__BR_X3,
  KGADGET_MOV_X10_X1__BR_X2,
  KGADGET_MOV_X20_X3__BR_X2,
  KGADGET_MOV_X13_X1__BR_X2,     
  KGADGET_MOV_X11_X13__BR_X10,   
  KGADGET_MOV_X7_X1__BR_X8,
  KGADGET_MOV_X0_X3__BR_X4,
  KGADGET_MOV_X5_X8__BR_X10,
  KGADGET_MOV_X0_X20__BR_X11,   
  KGADGET_MOV_X12_X0__BR_X2,
  KGADGET_MOV_X0_X1__BR_X2,
  KGADGET_MOV_X20_X15__BR_X12,
  KGADGET_MOV_X11_X1__BR_X12,
  KGADGET_MOV_X16_X1__BR_X2,
  KGADGET_MOV_X10_X12__BR_X8,
  KGADGET_MOV_X7_X16__BR_X10,
  KGADGET_MOV_X10_X0__BR_X2,
};

enum ksymbol {
  KSYMBOL_KERNPROC,
  KSYMBOL_RET_300,
  KSYMBOL_KFREE,
  KSYMBOL_PANIC,
  KSYMBOL_KALLOC_EXT,
  KHEAP_DATA_BUFFERS,
  KHEAP_DEFAULT,
  KHEAP_KEXT,
  necp_client_add_site,
  KSYMBOL_IOMalloc,
  KSYMBOL_IOFree,
  __ZZ17IOMalloc_internalE4site,
  pipespace_site,
  KSYMBOL_KFREE_EXT,
  KSYMBOL_KERNEL_PMAP,
  KSYMBOL_MMU_KVTOP,
  KSYMBOL_PMAP_VTOPHYS,
  KSYMBOL_PHYSTOKV,
  KSYMBOL_ml_phys_read_data,
  KSYMBOL_ptov_table,
  KSYMBOL_gPhysBase,
  KSYMBOL_gVirtBase,
  KSYMBOL_cpu_ttep,
  KSYMBOL_pmap_find_pa,
  KSYMBOL_pmap_find_phys,
};

extern uint32_t off_p_pid;
extern uint32_t off_p_pfd;
extern uint32_t off_p_list_le_prev;
extern uint32_t off_p_task;
extern uint32_t off_task_itk_space;
extern uint32_t off_fd_ofiles;
extern uint32_t off_fp_fglob;
extern uint32_t off_fg_data;
extern uint32_t off_pb_buffer;
extern uint32_t off_ipc_space_is_table;
extern uint32_t off_ipc_space_is_task;
extern uint32_t off_ipc_port_ip_receiver;
extern uint32_t off_ipc_port_ip_kobject;
extern uint32_t off_ipc_port_ikmq_base;
extern uint32_t off_ipc_kmsg_ikm_header;
extern uint32_t off_ipc_kmsg_ikm_data;
extern uint32_t off_mach_msg_header_t_msgh_remote_port;
extern uint32_t off_task_map; 
extern uint32_t off_vm_map_pmap;
extern uint32_t off_pmap_ttep;
extern uint32_t off_pmap_min;
extern uint32_t off_pmap_max;
extern uint32_t off_socket_so_pcb;

uint64_t kgad(enum kgadget sym);
uint64_t ksym(enum ksymbol sym);
void offsets_init(void);