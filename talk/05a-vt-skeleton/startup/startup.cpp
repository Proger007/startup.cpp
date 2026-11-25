#include <cstdint>

#include "Base.h"
#include "Board.h"

void Reset_Handler()
{
    main();
}

void Default_Handler()
{
    while (true) {}
}

[[gnu::section(".isr_vector"), gnu::used]]
alignas(Board::CoreTraits::VectorTableAlignment)
constexpr auto vector_table = Board::VectorTable::generate(std::make_index_sequence<Board::CoreTraits::VectorTableSize>{});
