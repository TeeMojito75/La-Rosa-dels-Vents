#ifndef SPRITE_COMPONENT_HPP
#define SPRITE_COMPONENT_HPP

#include <cstdint>

// Plantilla temporal
struct SpriteComponent {
    int oamId = -1;      // ID a OAM
    int tileIndex = 0;   // index base del tile
    int palette = 0;
    int size = 16;       // 8, 16, 32, 64
    bool flipX = false;
    bool flipY = false;
};

#endif
