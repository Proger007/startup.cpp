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
// Static constructors
extern startup::sections::FuncPtr __preinit_array_start, __preinit_array_end;
extern startup::sections::FuncPtr __init_array_start, __init_array_end;


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
    using preinit_array = startup::sections::CallableSection<&__preinit_array_start, &__preinit_array_end>;
    using init_array = startup::sections::CallableSection<&__init_array_start, &__init_array_end>;

    using SectionsInitSequence = startup::sections::Runner<
        startup::sections::Zero<bss>,
        startup::sections::Copy<data>,
        startup::sections::Call<preinit_array>,
        startup::sections::Call<init_array>
    >;
};

using Board = NucleoF411;
