# Talk stages

One directory per step of the talk. Each stage is a self-contained snapshot of the code shown on its slides, and builds for both boards.
Shared pieces: `common/` (CMake glue, presets, vendor startup files, capture script), plus `../cmake/boards.cmake`, `../gcc-arm-none-eabi.cmake`, `../Drivers/` and the linker scripts in `../bsp/`.

| Stage | Talk section | Slides | What it adds |
|---|---|---|---|
| `00-vendor-baseline` | §1, §2, §9 | 3, 6, 49, 51–52 | vendor startup + SystemInit, for dissection and the footprint comparison |
| `01-vendor-typo` | §2 | 6 | `TIM2_IRQhandler` typo: clean build, handler never runs (STM32F411 only) |
| `05a-vt-skeleton` | §5 | 13–18, 25–27 | enums, traits, generator without checks, stub startup.cpp |
| `05b-vt-bindings` | §5 | 23, 28 | `IsrBind` with real handlers, typo demos |
| `05c-vt-checks` | §5 | 19–22 | consteval range and uniqueness checks, the generator |
| `06a-early-hook` | §6 | 30–32 | optional `System::InitEarly`, vendor `SystemInit`, silent-typo demo |
| `07a-sections` | §7 | 37–41, 43–45 | sections as types, `-fno-builtin` vs builtins |
| `07b-copy-bench` | §7 | 46–47 | DWT benchmark and code size: loops vs `memcpy`/`memset` |
| `08-full` | §7, §8, §9 | 42, 48–49, 51–52 | everything, including `.init_array` and a global constructor |
| `10-minimal` | §10 | 54–57 | `ZeroTraits`, empty runner, with and without LTO |

Sections and slide numbers follow the 59-slide deck of 2026-11-03 (version 99): §0 intro, §1 what we have now, §2 limitations,
§3 life before `main`, §4 requirements, §5 vector table, §6 early init hook, §7 memory initialization,
§8 putting it all together, §9 what do we win, §10 squeezing. Each stage README also gives the plan ids (S-xx), which do not change when slides move.

Not created, as agreed: an explicit early-init policy stage, a complex-layout stage, a trampoline stage (§10 only mentions it).

## Build

Requires `arm-none-eabi-gcc` (tested with 13.2.1 and 15.3.1), CMake ≥ 3.23, Ninja. The code is C++20. The devcontainer/docker image in the repo has all three.

```sh
cd talk/05b-vt-bindings
cmake --preset stm32f411 && cmake --build --preset stm32f411
cmake --list-presets                       # all boards and variants of this stage
```

Collect everything for the slides into `talk/artifacts/<stage>/<preset>/`:

```sh
talk/common/scripts/capture.sh all
talk/common/scripts/capture.sh talk/05c-vt-checks stm32f411-alias
```

Build options (cache variables, set by the presets): `STARTUP_BOARD`, `STARTUP_LIBC` (nano/full/none), `STARTUP_BUILTINS`, `STARTUP_LTO`, `STARTUP_VARIANT`.
