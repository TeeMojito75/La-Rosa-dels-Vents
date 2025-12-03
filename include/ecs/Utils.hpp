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

namespace ecs {

    // Identificador d'entitats
    using EntityId = std::uint16_t;

    // Límits del motor ECS (ajusta segons el teu joc)
    constexpr std::size_t MAX_ENTITIES   = 256;  // quantes entitats com a màxim
    constexpr std::size_t MAX_COMPONENTS = 32;   // quants tipus de components diferents
    constexpr std::size_t MAX_SYSTEMS    = 16;   // quants tipus de sistemes diferents

    // Firma d'entitat: quins components té
    using Signature = std::bitset<MAX_COMPONENTS>;

    // Identificador de tipus (component, sistema, etc.)
    using TypeId = std::uint8_t; // suficient si MAX_COMPONENTS, MAX_SYSTEMS <= 256

    // Generador global d'IDs de tipus (només per components/sistemes)
    inline TypeId GenTypeId()
    {
        static TypeId last = 0;
        assert(last < MAX_COMPONENTS && "Too many component/system types registered!");
        return last++;
    }

    // Per a cada T, això retorna un ID únic i compacte 0..N-1
    template<typename T>
    inline TypeId get_type_id()
    {
        static TypeId id = GenTypeId();
        return id;
    }

    class Entity;
}

#endif