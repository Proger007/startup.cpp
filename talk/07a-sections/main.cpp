#include <cstdint>

volatile uint32_t counter;                 // .bss
volatile uint32_t increment = 0x1234'5678; // .data

[[noreturn]] int main()
{
    while (true)
    {
        counter += increment;
    }
}
