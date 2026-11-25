#pragma once

#include "STM32F4xxTraits.h"
#include "STM32F4xxSystem.h"
#include "VectorTable.h"
#include "Sections.h"
#include "Base.h"

#include "Handlers.h"

extern uintptr_t _estack; // NOLINT(*-reserved-identifier)

// Standard .data
extern uint32_t _sidata, _sdata, _edata;
// Standard .bss
extern uint32_t _sbss, _ebss;


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

    using bss = startup::sections::VmaSection<&_sbss, &_ebss>;
    using data = startup::sections::LmaVmaSection<&_sidata, &_sdata, &_edata>;

    using SectionsInitSequence = startup::sections::Runner<
        startup::sections::Zero<bss>,
        startup::sections::Copy<data>
    >;
};

using Board = NucleoF411;
