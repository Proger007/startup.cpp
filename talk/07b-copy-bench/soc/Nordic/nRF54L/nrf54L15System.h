//
// Created by root on 12/25/25.
//

#pragma once

#include "system_nrf54l.h"

namespace soc::nordic::nrf54l15::system
{
    struct System
    {
        static constexpr auto InitEarly = SystemInit;
    };
}
