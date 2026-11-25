//
// Created by Altaero on 12/6/25.
//

#pragma once

#include <array>
#include <cstdint>
#include <algorithm>
#include <type_traits>
#include <utility>

namespace startup::vector_table
{
    // A generic function pointer type for ISRs
    using IsrHandler = void (*)();

    // A binary-compatible union that avoids reinterpret_cast in constexpr generator
    union VectorEntry
    {
        uintptr_t* stack_pointer;
        IsrHandler handler;

        explicit constexpr VectorEntry(uintptr_t* sp) : stack_pointer(sp) {}
        explicit constexpr VectorEntry(const IsrHandler h) : handler(h) {}
    };

    // The Cortex-M architecture reserves the first 16 slots
    // (Stack Pointer + 15 Exceptions)
    constexpr int IRQn_Offset = 16;

    // Helper to ensure strict compile-time integer conversion
    template<auto EnumVal>
    consteval int to_int()
    {
        return static_cast<int>(EnumVal);
    }

    /**
     * @brief Represents a single binding of an IRQ number to a Handler function.
     * @tparam IRQn   The IRQ number (can be negative for System Faults, e.g., -14 for NMI).
     * @tparam Handler  The function to call.
     */
    template<auto IRQn, IsrHandler Handler>
    struct IsrBind
    {
        static constexpr auto irq = to_int<IRQn>();
        static constexpr auto handler = Handler;
    };

    /**
     * @brief Requirements for the SystemTraits class provided by the user/SoC definition.
     */
    template<typename T>
    concept SystemTraits = requires
    {
        { T::VectorTableSize } -> std::convertible_to<std::size_t>;
        { T::VectorTableAlignment } -> std::convertible_to<std::size_t>;
    };


    /**
     * @brief The Generator class that assembles the table.
     * @tparam Traits       SoC-specific traits (Size, Alignment).
     * @tparam StackTop     Pointer/Address of the Initial Stack Pointer (MSP).
     * @tparam ResetHandler The entry point of the application.
     * @tparam DefaultHandler The fallback handler for unmapped IRQs.
     * @tparam Bindings     Variadic list of IsrMap entries.
     */
    template<
        SystemTraits Traits,
        uintptr_t* StackTop,
        IsrHandler ResetHandler,
        IsrHandler DefaultHandler,
        typename... Bindings>
    struct Table
    {
        static consteval IsrHandler get_handler_for_index(const std::size_t index)
        {
            IsrHandler found = DefaultHandler;
            ((void)(Bindings::irq == (static_cast<int>(index) - IRQn_Offset) ? found = Bindings::handler : nullptr), ...);

            return found;
        }

        template<std::size_t... Index>
        static constexpr std::array<VectorEntry, Traits::VectorTableSize> generate(std::index_sequence<Index...>)
        {
            return
            {{
                (Index == 0) ? VectorEntry(StackTop) :
                (Index == 1) ? VectorEntry(ResetHandler) :
                VectorEntry(get_handler_for_index(Index))
                ...
            }};
        }
    };
}
