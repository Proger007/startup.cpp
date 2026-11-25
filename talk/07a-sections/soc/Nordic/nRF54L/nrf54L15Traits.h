//
// Created by root on 12/25/25.
//

#pragma once

#include "CortexM33Traits.h"

#include <bit>

enum class nRF54L15IRQn : int
{
    SWI00 = 28,
    SWI01 = 29,
    SWI02 = 30,
    SWI03 = 31,
    SPU00 = 64,
    MPC00 = 65,
    AAR00_CCM00 = 70,
    AAR00 = 70,
    CCM00 = 70,
    ECB00 = 71,
    CRACEN = 72,
    SERIAL00 = 74,
    RRAM = 75,
    VPR00 = 76,
    CTRLAP = 82,
    CM33SS = 83,
    TIMER00 = 85,
    SPU10 = 128,
    TIMER10 = 133,
    RTC10 = 134,
    EGU10 = 135,
    AAR10_CCM10 = 136,
    AAR10 = 136,
    CCM10 = 136,
    ECB10 = 137,
    RADIO_0 = 138,
    RADIO_1 = 139,
    SPU20 = 192,
    SERIAL20 = 198,
    SERIAL21 = 199,
    SERIAL22 = 200,
    EGU20 = 201,
    TIMER20 = 202,
    TIMER21 = 203,
    TIMER22 = 204,
    TIMER23 = 205,
    TIMER24 = 206,
    PWM20 = 210,
    PWM21 = 211,
    PWM22 = 212,
    SAADC = 213,
    NFCT = 214,
    TEMP = 215,
    GPIOTE20_1 = 219,
    TAMPC = 220,
    I2S20 = 221,
    QDEC20 = 224,
    QDEC21 = 225,
    GRTC_0 = 226,
    GRTC_1 = 227,
    GRTC_2 = 228,
    GRTC_3 = 229,
    SPU30 = 256,
    SERIAL30 = 260,
    CLOCK_POWER = 261,
    COMP_LPCOMP = 262,
    WDT30 = 264,
    WDT31 = 265,
    GPIOTE30_1 = 269,
    IRQn_MAX
};

struct nRF54L15Traits : public CM33Traits
{
    static constexpr std::size_t VectorTableSize = CM33Traits::VectorTableSize + static_cast<int>(nRF54L15IRQn::IRQn_MAX);
    static constexpr std::size_t VectorTableAlignment = std::bit_ceil(VectorTableSize * sizeof(std::uintptr_t));
};
