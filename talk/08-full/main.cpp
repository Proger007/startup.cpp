#include <cstdint>

volatile uint32_t counter;                 // .bss
volatile uint32_t increment = 0x1234'5678; // .data

struct Blinky
{
    uint32_t period;
    Blinky() : period(increment) {}        // runs from .init_array
};

Blinky blinky;

[[noreturn]] int main()
{
    while (true)
    {
        counter += blinky.period;
    }
}
