#pragma once

#include "nrf54L15Traits.h"
#include "VectorTable.h"
#include "Base.h"

#include "Handlers.h"

extern uintptr_t __StackTop; // NOLINT(*-reserved-identifier)


struct nRF54L15DK
{
    using CoreTraits = nRF54L15Traits;

    using VectorTable = startup::vector_table::Table<
        CoreTraits,
        &__StackTop,
        Reset_Handler,
        Default_Handler,
        startup::vector_table::IsrBind<nRF54L15IRQn::IRQn_MAX, timer00_handler>>;
};

using Board = nRF54L15DK;
