#pragma once

#include "nrf54L15Traits.h"
#include "nrf54L15System.h"
#include "VectorTable.h"
#include "Sections.h"
#include "Base.h"

#include "Handlers.h"

extern uintptr_t __StackTop; // NOLINT(*-reserved-identifier)

// Standard .data
extern uint32_t __etext, __data_start__, __data_end__;
// Standard .bss
extern uint32_t __bss_start__, __bss_end__;


struct nRF54L15DK
{
    using CoreTraits = nRF54L15Traits;

    using VectorTable = startup::vector_table::Table<
        CoreTraits,
        &__StackTop,
        Reset_Handler,
        Default_Handler,
        startup::vector_table::IsrBind<nRF54L15IRQn::TIMER00, timer00_handler>>;

    using System = soc::nordic::nrf54l15::system::System;

    using bss = startup::sections::VmaSection<&__bss_start__, &__bss_end__>;
    using data = startup::sections::LmaVmaSection<&__etext, &__data_start__, &__data_end__>;

    using SectionsInitSequence = startup::sections::Runner<
        startup::sections::Zero<bss>,
        startup::sections::Copy<data>
    >;
};

using Board = nRF54L15DK;
