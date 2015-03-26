#include <inttypes.h>
#include "pmm.h"
#include "kernel.h"
#include "stdlib.h"

#include "console.h"

#define BITMAP_SIZE 32768

extern const void kernelStart;
extern const void kernelEnd;

/**
 * Stores for every page in 4 GB RAM if it is free or not.
 * A set bit indicates that the page is free.
 */
static uint32_t bitmap[BITMAP_SIZE];

static inline void resolve(void *pptr, uint32_t *idx, uint32_t *bit)
{
    uintptr_t ptr = (uintptr_t)pptr;
    ptr >>= 12; // Calculate page ID

    *idx = ptr / 32; // B32 offset in bitmap.
    *bit = ptr % 32; // Bit offset in bitmap
}

static inline void *rebuild(uint32_t idx, uint32_t bit)
{
    uintptr_t ptr = 32 * idx + bit;
    ptr <<= 12;
    return (void*)ptr;
}

void pmm_init(const MultibootStructure *mb)
{
    if((mb->flags & MB_MEMORYMAP) == 0)
        die("Multiboot header missing memory map. Cannot initialize PMM.");

    memset(bitmap, 0, sizeof(bitmap));

    // Free the memory map
    {
        uintptr_t it = mb->memoryMap;
        for(size_t i = 0; i < mb->memoryMapLength; i++)
        {
            const MultibootMemoryMap *mmap = (const MultibootMemoryMap *)it;
            if(mmap->type == 1)
            {
                uintptr_t addr = (uintptr_t)mmap->base;
                uintptr_t end_addr = (uintptr_t)addr + (uintptr_t)mmap->length;
                while (addr < end_addr)
                {
                    pmm_free((void*)addr);
                    addr += 0x1000;
                }
            }
            it += mmap->entry_size + 4; // Stupid offset :P
        }
    }

    // Mark the whole kernel as "used"
    {
        uintptr_t it = (uintptr_t)&kernelStart;
        uintptr_t end = (uintptr_t)&kernelEnd;
        while(it < end)
        {
            uint32_t bit, idx;
            resolve((void*)it, &idx, &bit);
            bitmap[idx] &= ~(1<<bit);
            it += 0x1000;
        }
    }
}

uint32_t pmm_calc_free(void)
{
    uint32_t mem = 0;
    for(uint32_t idx = 0; idx < BITMAP_SIZE; idx++)
    {
        for(uint32_t bit = 0; bit < 32; bit++)
        {
            if(bitmap[idx] & (1<<bit))
                mem += 0x1000;
        }
    }
    return mem;
}

void pmm_free(void *pptr)
{
    uint32_t bit, idx;
    resolve(pptr, &idx, &bit);
    bitmap[idx] |= (1<<bit);
}


void *pmm_alloc(void)
{
    for(uint32_t idx = 0; idx < BITMAP_SIZE; idx++)
    {
        for(uint32_t bit = 0; bit < 32; bit++)
        {
            if(bitmap[idx] & (1<<bit))
            {
                // Allocate here
                bitmap[idx] &= ~(1<<bit);
                return rebuild(idx, bit);
            }
        }
    }
    die("out of physical memory");
    return (void*)0xDEADBEEF;
}
