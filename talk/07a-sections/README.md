# 07a · memory sections as types (§7)

Slides 37 (S-50: Board.h ↔ linker script), 38–41 (S-46, S-47, S-49, S-48: regions, linker symbols, the runner, `Copy`/`Zero`/`Call`), 43 (S-51) and 44–45 (S-52, S-53: the builtins surprise).
Slide 42 (S-49b, the full runner) comes from 08-full.

`Sections.h`: concepts, section models bound to linker symbols, `Zero` / `Copy`, and the `Runner`. Board.h declares `.bss` and `.data`; startup.cpp gets one line.

- default presets: built with `-fno-builtin`, the copy and zero loops stay loops (A-23)
- `-builtins`: without `-fno-builtin` (and with `-ftree-loop-distribute-patterns`), GCC 13.2.1 turns the loops into `memset`/`memcpy` calls and newlib's versions are linked in (A-22).
  GCC 15.3.1 at `-Os` keeps the loops and does not reproduce it, so slide 44 is labelled GCC 13.2.1.
