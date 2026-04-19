#include <stdint.h>
#include <mach/mach.h>
#include <CoreFoundation/CoreFoundation.h>

#define PROC_PIDREGIONINFO (7)
#define VM_KERN_MEMORY_OSKEXT (5)
#define LOADED_KEXT_SUMMARY_HDR_NAME_OFF (0x10)
#define LOADED_KEXT_SUMMARY_HDR_ADDR_OFF (0x60)
#define kOSBundleLoadAddressKey "OSBundleLoadAddress"

int
proc_pidinfo(int, int, uint64_t, void *, int);

CFDictionaryRef
OSKextCopyLoadedKextInfo(CFArrayRef, CFArrayRef);

kern_return_t
mach_vm_read_overwrite(vm_map_t, mach_vm_address_t, mach_vm_size_t, mach_vm_address_t, mach_vm_size_t *);

kern_return_t
mach_vm_write(vm_map_t, mach_vm_address_t, vm_offset_t, mach_msg_type_number_t);

kern_return_t
mach_vm_machine_attribute(vm_map_t, mach_vm_address_t, mach_vm_size_t, vm_machine_attribute_t, vm_machine_attribute_val_t *);

kern_return_t mach_vm_allocate(task_t task, mach_vm_address_t *addr, mach_vm_size_t size, int flags);

kern_return_t mach_vm_deallocate(task_t task, mach_vm_address_t addr, mach_vm_size_t size);


kern_return_t tfp0_init(void);
void tfp0_deinit(void);

int tfp0_get_kernel_base(void);

kern_return_t
tfp0_kreadbuf(uint64_t kaddr, void *buf, size_t sz);
kern_return_t
tfp0_kwritebuf(uint64_t kaddr, const void *buf, size_t sz);

uint8_t tfp0_kread8(uint64_t where);
uint16_t tfp0_kread16(uint64_t where);
uint32_t tfp0_kread32(uint64_t where);
uint64_t tfp0_kread64(uint64_t where);
void tfp0_kwrite8(uint64_t where, uint8_t what);
void tfp0_kwrite16(uint64_t where, uint16_t what);
void tfp0_kwrite32(uint64_t where, uint32_t what);
void tfp0_kwrite64(uint64_t where, uint64_t what);
uint64_t tfp0_kalloc(size_t sz);
void tfp0_kfree(uint64_t kaddr, size_t sz);

void tfp0_khexdump(uint64_t addr, size_t size);
