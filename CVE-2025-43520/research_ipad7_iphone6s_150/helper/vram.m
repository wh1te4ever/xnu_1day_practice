#import <Foundation/Foundation.h>
#import <mach/mach.h>

#import "vram.h"
#import "iokit.h"

bool get_vram_info(uint64_t *outPA, uint64_t *outSize)
{
    io_registry_entry_t entry = IORegistryEntryFromPath(
        kIOMainPortDefault, "IODeviceTree:/vram");
    if (entry == MACH_PORT_NULL) return false;

    CFDataRef reg = (CFDataRef)IORegistryEntryCreateCFProperty(
        entry, CFSTR("reg"), kCFAllocatorDefault, 0);
    IOObjectRelease(entry);

    if (!reg) return false;

    const uint64_t *v = (const uint64_t *)CFDataGetBytePtr(reg);
    *outPA   = v[0];
    *outSize = v[1];

    CFRelease(reg);
    return true;
}