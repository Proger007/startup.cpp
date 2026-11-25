#pragma once

#include "STM32F4xxTraits.h"
#include "VectorTable.h"
#include "Base.h"

#include "Handlers.h"

extern uintptr_t _estack; // NOLINT(*-reserved-identifier)


struct NucleoF411
{
    using CoreTraits = STM32F4xxTraits;

    using VectorTable = startup::vector_table::Table<
        CoreTraits,
        &_estack,
        Reset_Handler,
        Default_Handler,
        startup::vector_table::IsrBind<STM32F4xxIRQn::PVD, tim2_handler>,
        startup::vector_table::IsrBind<STM32F4xxIRQn::EXTI16, tim2_handler>>;
};

using Board = NucleoF411;
