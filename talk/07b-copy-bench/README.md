# 07b · copy/zero benchmark (§7)

Slides 46 (S-54: speed) and 47 (S-54b: size). Measured with GCC 15.3.1.

Measures a 16 KiB copy (flash → RAM) and a 16 KiB zero with DWT `CYCCNT`: an explicit word loop (what `Copy<>`/`Zero<>` compile to)
against the C library's `memcpy`/`memset`, newlib-nano in the `-nano` presets and full newlib in the `-full` presets.
Caches on, full speed: STM32F411 at 100 MHz (3 wait states, ART prefetch + I/D cache), nRF54L15 at 128 MHz (ICACHE on).

Flash each preset, run, halt after a few iterations and read (A-24):
- `results[0..3]`: copy loop, `memcpy`, zero loop, `memset` (cycles)
- `core_clock_hz`, `copy_ok` (must be 1)

Code size of each variant (slide 47): `memcpy`/`memset` and the `copy_loop`/`zero_loop` sizes in `symbols.txt` and the `.map` files.
