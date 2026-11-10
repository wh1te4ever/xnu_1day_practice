#include <stdint.h>
#include <mach/mach.h>

uint64_t tfp0_find_port(mach_port_name_t port);
uint64_t find_oolports_from_port(mach_port_name_t port);
uint64_t find_kmsgdata_from_port(mach_port_name_t port);
uint64_t port_to_kobject(mach_port_t port);