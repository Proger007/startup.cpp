//
// Created by root on 12/20/25.
//

#pragma once

#include <cstdint>
#include <concepts>

namespace startup::sections
{
    // 1. Data concepts. Define constraints on memory regions

    /// @brief Defines Virtual Memory Section.
    template<typename T>
    concept VirtualMemory = requires
    {
        { T::vma() } -> std::convertible_to<uint32_t*>; ///< Virtual Memory Address Start.
        { T::end() } -> std::convertible_to<uint32_t*>; ///< Virtual Memory Address End.
    };

    template<typename T>
    concept LoadableMemory = VirtualMemory<T> && requires
    {
        { T::lma() } -> std::convertible_to<uint32_t*>;
    };

    using FuncPtr = void (*)();
    template<typename T>
    concept FunctionTable = requires
    {
        { T::start() } -> std::convertible_to<FuncPtr*>;
        { T::end() } -> std::convertible_to<FuncPtr*>;
    };

    // 2. Action concepts. Define constraints on operations
    template<typename T>
    concept Runnable = requires
    {
        { T::run() } -> std::same_as<void>;
    };

    // 3. Section models.
    template<uint32_t* TLoad, uint32_t* TVirt, uint32_t* TEnd>
    struct LmaVmaSection
    {
        static constexpr uint32_t* lma() { return TLoad; }
        static constexpr uint32_t* vma() { return TVirt; }
        static constexpr uint32_t* end() { return TEnd; }
    };

    template<uint32_t* TVirt, uint32_t* TEnd>
    struct VmaSection
    {
        static constexpr uint32_t* vma() { return TVirt; }
        static constexpr uint32_t* end() { return TEnd; }
    };

    template<FuncPtr* TStart, FuncPtr* TEnd>
    struct CallableSection
    {
        static constexpr FuncPtr* start() { return TStart; }
        static constexpr FuncPtr* end() { return TEnd; }
    };

    // 4. Operations.
    template<LoadableMemory TSection>
    struct Copy
    {
        __attribute__((always_inline))
        static inline void run()
        {
            auto src = static_cast<std::uint32_t*>(__builtin_assume_aligned(TSection::lma(), sizeof(std::uint32_t)));
            auto dst = static_cast<std::uint32_t*>(__builtin_assume_aligned(TSection::vma(), sizeof(std::uint32_t)));
            const auto end = static_cast<std::uint32_t*>(__builtin_assume_aligned(TSection::end(), sizeof(std::uint32_t)));
            while (dst != end)
            {
                *dst++ = *src++;
            }
        }
    };

    template<VirtualMemory TSection>
    struct Zero
    {
        __attribute__((always_inline))
        static inline void run()
        {
            auto dst = static_cast<std::uint32_t*>(__builtin_assume_aligned(TSection::vma(), sizeof(std::uint32_t)));
            const auto end = static_cast<std::uint32_t*>(__builtin_assume_aligned(TSection::end(), sizeof(std::uint32_t)));
            while (dst != end)
            {
                *dst++ = 0;
            }
        }
    };

    template<FunctionTable TSection>
    struct Call
    {
        __attribute__((always_inline))
        static inline void run()
        {
            auto func = TSection::start();
            const auto end = TSection::end();

            while (func != end)
            {
                (*func)();
                ++func;
            }
        }
    };

    // 5. The runner engine.

    template<Runnable... Operations>
    struct Runner
    {
        __attribute__((always_inline))
        static inline void execute()
        {
            (Operations::run(), ...);
        }
    };
}
