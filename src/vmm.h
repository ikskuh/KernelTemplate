#pragma once

#include <inttypes.h>

#define VM_PRESENT  0x01
#define VM_WRITABLE 0x02
#define VM_USER     0x04

void vmm_init(void);

/**
 * Maps a physical address to a virtual address.
 * @param virtual The virtual address to be mapped.
 * @param physical Target physical address
 * @param flags Flags for the mapping
 * @remarks If flags is 0, the mapping will be undone.
 */
void vmm_map(uintptr_t virtual, uintptr_t physical, uint32_t flags);
