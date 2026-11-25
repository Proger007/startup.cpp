//
// Early init hook for STM32F4xx: the vendor CMSIS SystemInit (FPU enable, RCC reset state).
//

#pragma once

#include "system_stm32f4xx.h"

namespace soc::st::stm32f4::system
{
    struct System
    {
        static constexpr auto InitEarly = SystemInit;
    };
}
