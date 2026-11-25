#pragma once

#include "nrf54L15Traits.h"
#include "ZeroTraits.h"
#include "VectorTable.h"
#include "Sections.h"
#include "Base.h"

extern uintptr_t __StackTop; // NOLINT(*-reserved-identifier)

// Standard .data
extern uint32_t __etext, __data_start__, __data_end__;
// Standard .bss
extern uint32_t __bss_start__, __bss_end__;


struct nRF54L15DK
{
    using CoreTraits = ZeroTraits;

    using VectorTable = startup::vector_table::Table<
        CoreTraits,
        &__StackTop,
        Reset_Handler,
        Default_Handler>;

    using SectionsInitSequence = startup::sections::Runner<>;
};

using Board = nRF54L15DK;
