#pragma once

#include <nds.h>
#include "ecs/System.hpp"
#include "game/components/VelocityComponent.hpp"
#include "game/components/SpriteComponent.hpp"
#include "ecs/Utils.hpp"
#include "ecs/ComponentManager.hpp"

//Definim la constant de moviment del jugador
constexpr uint16 PLAYER_SPEED = 1;

class PlayerInputSystem : public ecs::System {
public:
    void Update(ecs::ComponentManager& cm) {
        // Habilitem la lectura dels botons
        scanKeys();
        uint16 keys = keysHeld();

        for (ecs::Entity e : mEntities) {
            auto& vel = cm.GetComponent<VelocityComponent>(e);
            auto& sp = cm.GetComponent<SpriteComponent>(e);

            vel.vx = 0;
            vel.vy = 0;

            // PRIORIDAD: horizontal > vertical
            // Setejem velocitat jugador i direcció cap on mira
            if (keys & KEY_UP) {
                vel.vy = -PLAYER_SPEED;
                sp.facing = Facing::Up;
            } else if (keys & KEY_DOWN) {
                vel.vy = PLAYER_SPEED;
                sp.facing = Facing::Down;
            } else if (keys & KEY_LEFT) {
                vel.vx = -PLAYER_SPEED;
                sp.facing = Facing::Left;
            } else if (keys & KEY_RIGHT) {
                vel.vx = PLAYER_SPEED;
                sp.facing = Facing::Right;
            }

            // Finalment actualitzam boolea moviment actual
            sp.moving = (vel.vx != 0 || vel.vy != 0);
        }
    }
};
