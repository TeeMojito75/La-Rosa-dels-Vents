#include <nds.h>
#include "game/GameScene.hpp"

int main() {
    consoleDemoInit();  // usaremos la consola para “renderizar” el mapa/cubo

    // Tilemap 32x24 tiles de 8px (borde sólido)
    static uint8_t map[32*24] = {};
    for (int x=0; x<32; ++x) { map[x] = 1; map[(24-1)*32 + x] = 1; }
    for (int y=0; y<24; ++y) { map[y*32] = 1; map[y*32 + (32-1)] = 1; }

    GameScene scene(map, 32, 24, 8);

    while (1) {
        swiWaitForVBlank();
        scene.update();
    }
    return 0;
}
