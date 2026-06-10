
#include <errno.h>
#include <mach/mach.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysctl.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdbool.h>

#include "device_info.h"


struct DeviceInfo get_device_info() {
    struct DeviceInfo info = {0};
    size_t size;

    size = sizeof(info.model);
    sysctlbyname("hw.machine", info.model, &size, NULL, 0);

    size = sizeof(info.version);
    sysctlbyname("kern.osproductversion", info.version, &size, NULL, 0);

    return info;
}
