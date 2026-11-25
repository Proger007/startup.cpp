//
// Created by Altaero on 12/23/25.
//

#pragma once

// Reset_Handler keeps C linkage by convention only: the core just reads the vector table, so a mangled
// C++ name works too. The plain name lets ENTRY(Reset_Handler) in the linker script and the debugger find
// it; with a mangled name, pass -Wl,-e,<mangled> instead, or ld warns and leaves e_entry wrong.
extern "C"
[[noreturn]] void Reset_Handler();

// We're not obliged to stick to C namings here and further, and can leave C++ mangling, but keep C ABI for sanity
extern "C"
[[noreturn]] void Default_Handler();

// Freestanding implementation: start-up is implementation-defined, so Reset_Handler calls main itself.
// [[noreturn]] must be on the first declaration, and it lets the compiler see that main never returns.
[[noreturn]] int main();
