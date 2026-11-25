#include <stdint.h>

volatile uint32_t counter;                 /* .bss  */
volatile uint32_t increment = 0x12345678;  /* .data */

int main(void)
{
    while (1)
    {
        counter += increment;
    }
}
