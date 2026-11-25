# 00 · vendor baseline (§1, §2, §9)

Slides 3 (O-1: the nRF54L15 startup file, size only), 6 (S-18), 49 (S-58: vendor `Reset_Handler` disassembly)
and 51–52 (S-59, S-59b: footprint and what the vendor builds carry). The ST `.s` file is no longer walked through on a slide.

The vendor way, for dissection slides and for the footprint comparison: vendor startup file + vendor `SystemInit` + the same `main` body as the C++ stages from 07a on.

- STM32F411: `startup_stm32f411xe.s` (CMSIS device F4), `system_stm32f4xx.c`, `STM32F411RETx_FLASH.ld`.
- nRF54L15: `gcc_startup_nrf54l15_application.S` + its own `nrf54l15_xxaa_application.ld` / `nrf_common.ld` (nrfx 4.6 MDK; its symbol names differ from the older `nrf_common.ld` in `bsp/`), `system_nrf54l.c`.

Vendor files: `../common/vendor/`. Presets: `-nano` (newlib-nano) and `-full` (full newlib).
Note the order: ST calls `SystemInit` before the copy/zero loops, Nordic copies `.data` first and calls `SystemInit` after.
