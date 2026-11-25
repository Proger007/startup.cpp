# 06a · early init hook (§6)

Slides 30–32 (S-42…S-44): the optional hook, why optional, the trade-off.

`try_soc_early_init` calls `Board::System::InitEarly()` if it exists. Both boards plug in the vendor C `SystemInit` (`system_stm32f4xx.c`, `system_nrf54l.c`).

- default presets: `Reset_Handler` calls `SystemInit` (A-19)
- `-typo-hook`: `InitEary` in the System header. Clean build, and `SystemInit` is silently gone from `Reset_Handler` (A-20)
