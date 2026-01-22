#include "game/inits/BackgroundRenderer.hpp"
#include "game/inits/GameScene.hpp"

int main() {
    BackgroundRenderer renderer;
    GameScene scene(renderer);

    if (!scene.init()) {
        while (1) swiWaitForVBlank();
    }

    scene.run();
    return 0;
}
