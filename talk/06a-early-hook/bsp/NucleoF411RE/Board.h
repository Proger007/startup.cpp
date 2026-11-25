#pragma once

#include "STM32F4xxTraits.h"
#include "STM32F4xxSystem.h"
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
        startup::vector_table::IsrBind<STM32F4xxIRQn::TIM2, tim2_handler>>;

    using System = soc::st::stm32f4::system::System;
};

using Board = NucleoF411;
