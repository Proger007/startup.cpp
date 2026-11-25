# 05a · vector table skeleton (§5)

Slides 13–18 (S-22…S-27: enums, binding, `VectorEntry`, `SystemTraits`, traits) and 25–27 (S-38…S-40: startup.cpp v1, first build, same bytes as the vendor table).

IRQ enums and traits, `VectorTable.h` without checks, a Board.h with no bindings, and a startup.cpp with stub handlers and the three-line table definition.
The image is about the size of the table: 102 entries (408 B) on STM32F411, 286 entries (1144 B) on nRF54L15 (A-10, A-11).
