// Copy/zero benchmark: an explicit word loop (what startup.cpp's Copy<>/Zero<> compile to) vs the C library's
// memcpy/memset, with the caches on and the core at full speed.
// Halt after a few iterations of the main loop and read in the debugger:
//   results[0] copy loop   results[1] memcpy   results[2] zero loop   results[3] memset   (DWT cycles, 16 KiB each)
//   core_clock_hz          the clock the numbers were taken at
//   copy_ok                1 when both copy variants produced the source pattern
// The loops opt out of GCC's loop-to-memcpy/memset rewrite themselves, so they stay loops whatever STARTUP_BUILTINS
// says; memcpy/memset are the C library's (newlib-nano in the -nano presets, full newlib in the -full presets).

#include <cstdint>
#include <cstring>

#if defined(STM32F411xE)
#include "stm32f4xx.h"
#elif defined(NRF54L15_XXAA)
#include "nrf.h"
#include "system_nrf54l.h"
#endif

namespace
{
    constexpr std::size_t Words = 4096; // 16 KiB

    struct Pattern
    {
        uint32_t words[Words];
        consteval Pattern() : words{}
        {
            for (std::size_t i = 0; i < Words; ++i) words[i] = 0x9E37'79B9u * (i + 1);
        }
    };

    alignas(4) constexpr Pattern source{};   // flash / RRAM
    alignas(4) uint32_t destination[Words];  // .bss

    // The explicit loops, word by word, as in Copy<>::run and Zero<>::run
    [[gnu::noinline, gnu::optimize("no-tree-loop-distribute-patterns")]]
    void copy_loop(const uint32_t* from, uint32_t* to, const uint32_t* const end)
    {
        while (to != end) *to++ = *from++;
    }

    [[gnu::noinline, gnu::optimize("no-tree-loop-distribute-patterns")]]
    void zero_loop(uint32_t* to, const uint32_t* const end)
    {
        while (to != end) *to++ = 0;
    }

    // Full speed, caches on
    uint32_t configure_core()
    {
#if defined(STM32F411xE)
        // 100 MHz from HSI: VOS scale 1, 3 wait states, ART accelerator (prefetch, I-cache, D-cache) on
        RCC->APB1ENR |= RCC_APB1ENR_PWREN;
        (void)RCC->APB1ENR;
        PWR->CR |= PWR_CR_VOS;
        FLASH->ACR = FLASH_ACR_LATENCY_3WS | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;
        RCC->PLLCFGR = (16u << RCC_PLLCFGR_PLLM_Pos) | (200u << RCC_PLLCFGR_PLLN_Pos) | (0u << RCC_PLLCFGR_PLLP_Pos)
                     | RCC_PLLCFGR_PLLSRC_HSI | (4u << RCC_PLLCFGR_PLLQ_Pos); // 16 MHz / 16 * 200 / 2
        RCC->CR |= RCC_CR_PLLON;
        while (!(RCC->CR & RCC_CR_PLLRDY)) {}
        RCC->CFGR = RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_SW_PLL;
        while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}
        return 100'000'000u;
#elif defined(NRF54L15_XXAA)
        // SystemInit already switched the CPU to 128 MHz; the instruction cache is off after reset
        NRF_ICACHE->ENABLE = CACHE_ENABLE_ENABLE_Enabled;
        SystemCoreClockUpdate();
        return SystemCoreClock;
#endif
    }

    constexpr uintptr_t DEMCR = 0xE000'EDFC;
    constexpr uintptr_t DWT_CTRL = 0xE000'1000;
    constexpr uintptr_t DWT_CYCCNT = 0xE000'1004;

    inline volatile uint32_t& reg(const uintptr_t address) { return *reinterpret_cast<volatile uint32_t*>(address); }

    template<typename F>
    uint32_t cycles(F&& f)
    {
        reg(DWT_CYCCNT) = 0;
        f();
        return reg(DWT_CYCCNT);
    }

    bool copied() { return std::memcmp(destination, source.words, sizeof(destination)) == 0; }
}

volatile uint32_t results[4];
volatile uint32_t core_clock_hz;
volatile uint32_t copy_ok;

[[noreturn]] int main()
{
    core_clock_hz = configure_core();

    reg(DEMCR) |= 1u << 24;   // TRCENA
    reg(DWT_CTRL) |= 1u;      // CYCCNTENA

    while (true)
    {
        results[0] = cycles([] { copy_loop(source.words, destination, destination + Words); });
        bool ok = copied();
        zero_loop(destination, destination + Words);
        results[1] = cycles([] { std::memcpy(destination, source.words, sizeof(destination)); });
        ok = ok && copied();
        results[2] = cycles([] { zero_loop(destination, destination + Words); });
        results[3] = cycles([] { std::memset(destination, 0, sizeof(destination)); });
        copy_ok = ok;
    }
}
