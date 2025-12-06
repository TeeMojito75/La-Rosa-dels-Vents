#pragma once

#include <nds.h>
#include "ecs/System.hpp"
#include "game/components/VelocityComponent.hpp"
#include "ecs/Utils.hpp"
#include "ecs/ComponentManager.hpp"

constexpr Fix PLAYER_SPEED = FIX_FROM_INT(2);

class PlayerInputSystem : public ecs::System
{
public:
    void Update(ecs::ComponentManager& cm)
    {
        scanKeys();
        int held = keysHeld();

        for (ecs::Entity e : mEntities)
        {
            auto& vel = cm.GetComponent<VelocityComponent>(e);

            vel.vx = FIX_ZERO;
            vel.vy = FIX_ZERO;

            bool left  = held & KEY_LEFT;
            bool right = held & KEY_RIGHT;
            bool up    = held & KEY_UP;
            bool down  = held & KEY_DOWN;

            // PRIORIDAD: horizontal > vertical
            if (left)
                vel.vx = -PLAYER_SPEED;
            else if (right)
                vel.vx =  PLAYER_SPEED;
            else if (up)
                vel.vy = -PLAYER_SPEED;
            else if (down)
                vel.vy =  PLAYER_SPEED;

        }
    }
};
