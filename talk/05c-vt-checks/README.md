# 05c · table checks (§5)

Slides 19–20 (S-28, S-29: the two checks) and 21–22 (S-31, S-32: the generator).

Same as 05b, with `checks::binding_in_range()` and `checks::binding_is_unique()` (consteval functions) in `VectorTable.h`.

Error demos (expected to fail):
- `-alias`: `PVD` + `EXTI16` on ST, `AAR00` + `CCM00` on nRF: different names, one IRQ (A-18, slide 20)
- `-out-of-range`: binds `IRQn_MAX`, one past the last slot (A-15)
- `-below-nmi`: binds -15, the Reset slot (A-16)
- `-duplicate`: the same IRQ bound twice (A-17)

A-15…A-17 are still captured, but the slide that showed them was cut; keep them for Q&A.

Not caught (worth knowing for Q&A): `IsrBind<auto IRQn, …>` accepts any enum, so an IRQ from another vendor's enum that happens to be in range compiles.
