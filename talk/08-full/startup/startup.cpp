#include <cstdint>

#include "Base.h"
#include "Board.h"

namespace {
    template <typename T>
    __attribute__((always_inline))
    inline void try_soc_early_init()
    {
        if constexpr (requires { T::System::InitEarly(); })
        {
            T::System::InitEarly();
        }
    }
}

void Reset_Handler()
{
    try_soc_early_init<Board>();

    Board::SectionsInitSequence::execute();

    main();
}

void Default_Handler()
{
    while (true) {}
}

[[gnu::section(".isr_vector"), gnu::used]]
alignas(Board::CoreTraits::VectorTableAlignment)
constexpr auto vector_table = Board::VectorTable::generate(std::make_index_sequence<Board::CoreTraits::VectorTableSize>{});
