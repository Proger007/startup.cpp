#pragma once

#include "STM32F4xxTraits.h"
#include "ZeroTraits.h"
#include "VectorTable.h"
#include "Sections.h"
#include "Base.h"

extern uintptr_t _estack; // NOLINT(*-reserved-identifier)

// Standard .data
extern uint32_t _sidata, _sdata, _edata;
// Standard .bss
extern uint32_t _sbss, _ebss;


struct NucleoF411
{
    using CoreTraits = ZeroTraits;

    using VectorTable = startup::vector_table::Table<
        CoreTraits,
        &_estack,
        Reset_Handler,
        Default_Handler>;

    using SectionsInitSequence = startup::sections::Runner<>;
};

using Board = NucleoF411;
