#include "game/systems/InputSystem.hpp"

extern "C" {
    #include <nds.h>
}

using namespace ecs;

void InputSystem::update(ComponentManager& components)
{
    scanKeys();
    uint32 keys = keysHeld();

    for (Entity entity : mEntities)
    {
        auto& input = components.GetComponent<InputComponent>(entity);

        input.up = keys & KEY_UP;
        input.down = keys & KEY_DOWN;
        input.left = keys & KEY_LEFT;
        input.right = keys & KEY_RIGHT;

        input.attack = keys & KEY_B;
        input.interact = keys & KEY_A;
    }
}
