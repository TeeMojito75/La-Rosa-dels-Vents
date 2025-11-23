#ifndef UTILS_UTILS_HPP
#define UTILS_UTILS_HPP
#include <bitset>
#include <cstdint>
#include <cassert>


// -------- Fixed Point 16.16 ---------
using Fix = int32_t;

constexpr int FIX_SHIFT = 16;
constexpr int FIX_ONE   = (1 << FIX_SHIFT);

constexpr Fix FIX_FROM_INT(int x) { return x << FIX_SHIFT; }
constexpr int FIX_TO_INT(Fix x)   { return x >> FIX_SHIFT; }

constexpr Fix FIX_FROM_FLOAT(float f) { return (Fix)(f * (float)FIX_ONE); }

constexpr Fix FIX_MUL(Fix a, Fix b)
{
    return (Fix)(((int64_t)a * (int64_t)b) >> FIX_SHIFT);
}

constexpr Fix FIX_DIV(Fix a, Fix b)
{
    return (Fix)(((int64_t)a << FIX_SHIFT) / b);
}

// ----- Helpers opcionals -----

constexpr Fix FIX_ZERO = (Fix)0;
constexpr Fix FIX_HALF = FIX_ONE >> 1;
constexpr Fix FIX_TWO  = FIX_ONE << 1;

// clamp
constexpr Fix FIX_CLAMP(Fix v, Fix lo, Fix hi) {
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

namespace ecs 
{
    // Aquests valors son ajustables depenent de les necessitats de mem
    using EntityId = std::uint32_t;
    inline constexpr EntityId MAX_ENTITIES = 1024;// Nombre màxim d'entitats


    using ComponentType = std::uint8_t;
    inline constexpr std::size_t MAX_COMPONENTS = 32;// nombre màxim de components

    using Signature = std::bitset<MAX_COMPONENTS>;

    // Identificador de tipus
    using TypeId = std::size_t;

    template<typename T>
    inline TypeId get_type_id() {
        static int unique_tag_for_T;
        return reinterpret_cast<TypeId>(&unique_tag_for_T);
    }

    class Entity;
}
#endif