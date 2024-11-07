#include "fixedpoint.h"
#include <stdio.h>

void snprintfix(char* dest, uint8_t size, fixed f)
{
    snprintf(dest, size, FIX_PRINT_FORMAT, f >> FRAC_BITS, 10000 * (uint32_t)(f & 0x0000ffff) / FIX_SCALE);
}

#ifdef ENABLE_SPRINTU64
char* sprintu64(uint64_t value)
{
    // Buffer large enough to hold maximum 64-bit value in base 10.
    static char buffer[24]; 

    // Make sure the string is null-terminated
    uint8_t pos = sizeof(buffer) - 1;   
    buffer[pos--] = 0;

    // Handle special case if value = 0
    if (value == 0) {
        buffer[pos] = '0';
        return &(buffer[pos]);
    }

    uint8_t temp;
    // Build the string backwards digit by digit
    while (value > 0) {
        temp = value % 10;
        buffer[pos--] = '0' + temp;
        value /= 10;
    }

    // Return the address of the final converted digit
    return &(buffer[pos + 1]);
}
#endif