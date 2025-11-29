/*
 * Performance measurement of the CRC code
 * Measures minimum cycles per byte in the cache-warm case
 * for sizes up to 64KB
 *
 * Works only with gcc and x86; x86-64 should work (not tested)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <memory.h>

#define ARCH_LITTLE_ENDIAN

#include "crc_code.c"

static inline void serialize()
{
    asm volatile ("xor %%eax, %%eax\n"
                  "cpuid\n" : : :
                  "%eax", "%ebx", "%ecx", "%edx");
}

static inline unsigned __tsc()
{
    uint32_t tmp;
    asm volatile ("rdtsc\n" : "=a" (tmp) : : "%edx");
    return (unsigned)tmp;
}

static inline unsigned tsc()
{
    unsigned tmp;
    serialize();
    tmp = __tsc();
    serialize();
    return tmp;
}

typedef void (*func_ptr_t)();

static unsigned __tsc_measure(func_ptr_t func)
{
    size_t i;
    unsigned ticks;
    unsigned min_ticks = UINT_MAX;

    for (i = 0; i < 32; i++) {
        ticks = tsc();
        func();
        ticks = tsc() - ticks;
        if (ticks < min_ticks) {
            min_ticks = (unsigned)ticks;
        }
    }

    return min_ticks;
}

static void __dummy()
{
}

static int tsc_measure(func_ptr_t func)
{
    return __tsc_measure(func) - __tsc_measure(&__dummy);
}

static unsigned char *crc_data;
static size_t crc_size;
static volatile uint64_t crc_result;

static void crc_func()
{
    crc_result = crc_calc(crc_data, crc_size);
}

static int crc_cycles(size_t size)
{
    void* mem;
    int cycles;
    size_t i;

    mem = malloc(size + 63);
    crc_data = (unsigned char*)(((uintptr_t)mem + 63) & ~((uintptr_t)63));
    for (i = 0; i < size; i++) crc_data[i] = rand() & 0xff;
    crc_size = size;
    cycles = tsc_measure(&crc_func);
    free(mem);

    return cycles;
}

int main()
{
    size_t size;
    double min_cycles = 1e6;
    double cycles_per_byte;

    for (size = 1; size <= (1 << 16); size <<= 1) {
        int cycles = crc_cycles(size);
        cycles_per_byte = (double)cycles / size;
        if (cycles_per_byte < min_cycles) min_cycles = cycles_per_byte;
    }

    printf("%.2f\n", min_cycles);

    return 0;
}
