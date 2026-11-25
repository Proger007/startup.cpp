//
// Created by Altaero on 12/8/25.
//

#pragma once

#include <cstdint>

enum class CM4IRQn : int
{
    NMI = -14,
    HardFault = -13,
    MemFault = -12,
    BusFault = -11,
    UsageFault = -10,
    SVCall = -5,
    DebugMon = -4,
    PendSV = -2,
    SysTick = -1
};

struct CM4Traits
{
    static constexpr std::size_t VectorTableSize = 16;
    static constexpr std::size_t VectorTableAlignment = 512;
};
