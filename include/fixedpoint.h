#ifndef FIXED_POINT_H
#define FIXED_POINT_H

#include <inttypes.h>

/**
 * By default the library uses 64 bit (32.32) fixed point, unless any of the 
 * following is defined:
 * FIX16_16: Use 32 bit (16.16) fixed point.
 * FIX8_8: Use 16 bit (8.8) fixed point.
 */

#if defined(FIX16_16)
typedef int16_t shortfixed;
typedef int32_t fixed;
typedef int64_t longfixed;
typedef int32_t integer;
#define FRAC_BITS 16
#define SHORT_SIGN_MASK 0x8000
#define FIX_PRINT_FORMAT "%" PRId16 ".%04" PRIu16
#elif defined(FIX8_8)
typedef int8_t shortfixed;
typedef int16_t fixed;
typedef int32_t longfixed;
typedef int16_t integer;
#define FRAC_BITS 8
#define SHORT_SIGN_MASK 0x80
#define FIX_PRINT_FORMAT "%" PRId8 ".%04" PRIu8
#else
typedef int16_t shortfixed;
typedef int32_t fixed;
typedef int64_t longfixed;
typedef int32_t integer;
#define FRAC_BITS 16
#define SHORT_SIGN_MASK 0x8000
#define FIX_PRINT_FORMAT "%" PRId16 ".%04" PRIu16
#endif

#define FIX_SCALE ((longfixed)1 << FRAC_BITS)

inline fixed int2fix(integer x) 
{
    return x * FIX_SCALE;
}

inline fixed float2fix(float x)
{
    return x * FIX_SCALE;
}

inline float fix2float(fixed x) {
    return (float)x / FIX_SCALE;
}

inline integer fix2int(fixed x)
{
    return (x + (1 << (FRAC_BITS - 1))) >> FRAC_BITS;
}

inline fixed shortfix2fix(shortfixed x) {
    return (fixed)x << (FRAC_BITS >> 1);
}

inline fixed fix_add(fixed x, fixed y)
{
    return x + y;
}

inline fixed fix_sub(fixed x, fixed y)
{
    return x - y;
}

inline fixed fix_mul(fixed x, fixed y)
{
    return ((longfixed)x * (longfixed)y  + (1 << (FRAC_BITS - 1))) >> FRAC_BITS;
}

inline fixed fix_div(fixed x, fixed y)
{
    return (longfixed)x * FIX_SCALE / y;
}

/**
 * Converts the fixed point number to a base-10 decimal number stored in a
 * char buffer.
 * @param dest Pointer to buffer that will hold the resulting string.
 * @param size The maximum size of the destination buffer.
 * @param f The fixed-point number to convert.
 */
void snprintfix(char* dest, uint8_t size, fixed f);

#ifdef ENABLE_SPRINTU64
/**
 * Converts a 64-bit unsigned value to a base-10 number stored in a char buffer.
 * This can be useful on systems which don't natively support 64-bit integers.
 * The pointer to the returned string returned points to a static buffer owned
 * by the function therefore this function is not thread-safe.
 * @param value A 64-bit value.
 * @return A null-terminated string in base 10 representing the value.
 */
char* sprintu64(uint64_t value);
#endif // #ifdef ENABLE_SPRINT64

#endif // #ifndef FIXED_POINT_H
