//
// Created by Altaero on 12/25/25.
//

#pragma once

#include <cstdint>

enum class CM33IRQn : int
{
    NMI = -14,
    HardFault = -13,
    MemFault = -12,
    BusFault = -11,
    UsageFault = -10,
    SecureFault = -9,
    SVCall = -5,
    DebugMon = -4,
    PendSV = -2,
    SysTick = -1
};

struct CM33Traits
{
    static constexpr std::size_t VectorTableSize = 16;
    static constexpr std::size_t VectorTableAlignment = 512;
};
