#ifndef ECS_TYPES_HPP
#define ECS_TYPES_HPP

#include <stdint.h>

namespace ecs {

    // Aquests valors son ajustables depenent de les necessitats de mem
    static constexpr uint16_t  MAX_ENTITIES = 2048; // Fins a 2048 entitats
    static constexpr uint8_t MAX_COMPONENTS = 32;   // Fins a 32 tipus de components

    using Entity        = uint16_t;  // Índex vàlid [0, MAX_ENTITIES) 
    using ComponentType = uint8_t;   // Id del tipus de component
    using Signature     = uint32_t;  // 1 per component fins a MAX_COMPONENTS

    inline constexpr Signature bit(ComponentType c) {
        return Signature(1u) << c;
    }
}

#endif 
