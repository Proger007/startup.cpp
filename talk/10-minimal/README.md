# 10 · squeezing the absolute most out of it (§10)

Slides 54 (S-61: the rules), 55 (S-61b: this stage's Board.h and main.cpp, nRF54L15), 56 (S-62: 16 bytes) and 57 (S-63: LTO).

startup.cpp is byte-identical to 08-full. Only Board.h and main.cpp change: `ZeroTraits` (SP + Reset only), an empty `Runner<>`, no `System`, and a `main` that is just a loop.

- default presets: no LTO, `Reset_Handler` calls `main` (A-29)
- `-lto`: everything collapses into the table and one branch-to-self (A-30). Compare the `.map` files for the 2-byte difference between the boards (A-31).
