# 08 · putting it all together (§8, §9)

Slides 42 (S-49b: the nRF54L15 runner), 48 (S-56: the whole startup.cpp), 49 (S-58: one instruction more) and 51–52 (S-59, S-59b: footprint).

Everything: bindings, early init hook, `Zero`/`Copy`/`Call` for `.bss`, `.data`, `.preinit_array` and `.init_array`, and a global object with a constructor.
On nRF54L15, `.init_array` sits inside `.data` in RAM, so `Call<init_array>` must come after `Copy<data>`; the type list says so.

Compare with 00-vendor-baseline (same flags, same libc) for the footprint table (A-26) and the `Reset_Handler` disassembly (A-27).
With GCC 15.3.1: 612 vs 792 B on STM32F411, 1664 vs 2168 B on nRF54L15. Slide 52 explains the difference: the vendor builds link the default start files and crt0, startup.cpp builds with `-nostartfiles`.
