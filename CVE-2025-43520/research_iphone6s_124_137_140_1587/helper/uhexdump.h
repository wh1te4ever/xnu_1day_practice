#include <stdint.h>
#include <stdlib.h>

void uhexdump(uint64_t addr, size_t size);
void uhexdump_at(uint64_t display_addr, const void *src, size_t size);