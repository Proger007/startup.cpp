//
// Created by Altaero on 12/8/25.
//

#pragma once

#include "VectorTable.h"
#include "Sections.h"
#include "Base.h"
#include "nrf54L15System.h"
#include "nrf54L15Traits.h"

#include "nrf54_handler_dummy.h"

extern uintptr_t __StackTop; // NOLINT(*-reserved-identifier)

// Standard .data
extern uint32_t __etext, __data_start__, __data_end__;
// Standard .bss
extern uint32_t __bss_start__, __bss_end__;
// Static constructors, placed inside .data (RAM) by nrf_common.ld: call them only after Copy<data>
extern startup::sections::FuncPtr __preinit_array_start, __preinit_array_end;
extern startup::sections::FuncPtr __init_array_start, __init_array_end;


struct nRF54L15DK
{
    using CoreTraits = nRF54L15Traits;

    using VectorTable = startup::vector_table::Table<
        CoreTraits,
        &__StackTop,
        Reset_Handler,
        Default_Handler,
        startup::vector_table::IsrBind<nRF54L15IRQn::TIMER00, TIMER00_Handler>>;

    using System = soc::nordic::nrf54l15::system::System;

    using bss = startup::sections::VmaSection<&__bss_start__, &__bss_end__>;
    using data = startup::sections::LmaVmaSection<&__etext, &__data_start__, &__data_end__>;
    using preinit_array = startup::sections::CallableSection<&__preinit_array_start, &__preinit_array_end>;
    using init_array = startup::sections::CallableSection<&__init_array_start, &__init_array_end>;

    using SectionsInitSequence = startup::sections::Runner<
        startup::sections::Zero<bss>,
        startup::sections::Copy<data>,
        startup::sections::Call<preinit_array>,
        startup::sections::Call<init_array>
    >;
};

using Board = nRF54L15DK;
