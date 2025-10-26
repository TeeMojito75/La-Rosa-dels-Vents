#ifndef ECS_TYPES_HPP
#define ECS_TYPES_HPP

#include <bitset>
#include <cstdint>
#include <assert.h>

namespace ecs 
{

    // Aquests valors son ajustables depenent de les necessitats de mem
    using Entity = std::uint32_t;
    const Entity MAX_ENTITIES = 1024; // Nombre màxim d'entitats


    using ComponentType = std::uint8_t;
    const std::size_t MAX_COMPONENTS = 32;// nombre màxim de components


    using Signature = std::bitset<MAX_COMPONENTS>;
}

#endif 
