#include <stdint.h>

struct kDictEntry {
    uint64_t key;
    uint64_t value;
};

struct kOSDict {
    uint64_t self_addr;
    uint64_t items_addr;
    uint32_t count;
    uint32_t cap;
    char **names;
    struct kDictEntry *items;
    char data[0];
};

uint64_t userdict_from_IOSurface(void);
struct kOSDict *kernel_fetch_dict(uint64_t dict_addr);