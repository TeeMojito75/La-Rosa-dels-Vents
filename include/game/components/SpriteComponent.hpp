#ifndef SPRITE_COMPONENT_HPP
#define SPRITE_COMPONENT_HPP

#include <cstdint>

// Plantilla temporal
struct SpriteComponent {
    int oamId = -1;      // ID a OAM
    int tileIndex = 0;   // índex base del tile
    int palette = 0;
    int size = 16;       
    bool flipX = false;
    bool flipY = false;
};

#endif
