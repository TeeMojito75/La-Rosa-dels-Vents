#ifndef ECS_UTILS_HPP
#define ECS_UTILS_HPP
#include <bitset>
#include <cstdint>
#include <cassert>

namespace ecs 
{
    // Aquests valors son ajustables depenent de les necessitats de mem
    using EntityId = std::uint32_t;
    inline constexpr EntityId MAX_ENTITIES = 1024;// Nombre màxim d'entitats


    using ComponentType = std::uint8_t;
    inline constexpr std::size_t MAX_COMPONENTS = 32;// nombre màxim de components

    using Signature = std::bitset<MAX_COMPONENTS>;

    // Identificador de tipo sin RTTI
    using TypeId = std::size_t;

    template<typename T>
    inline TypeId get_type_id() {
        static int unique_tag_for_T;
        return reinterpret_cast<TypeId>(&unique_tag_for_T);
    }

    class Entity;
}
#endif