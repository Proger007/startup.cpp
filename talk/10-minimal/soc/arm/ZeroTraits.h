//
// Created by Altaero on 12/8/25.
//

#pragma once

#include <cstdint>

struct ZeroTraits
{
    static constexpr std::size_t VectorTableSize = 2; // Stack pointer + Reset Handler
    static constexpr std::size_t VectorTableAlignment = 512;
};
