#pragma once

#include <nf_lib.h>
#include "ecs/System.hpp"
#include "ecs/ComponentManager.hpp"
#include "game/components/TransformComponent.hpp"
#include "game/components/SpriteComponent.hpp"

class SpriteSystem2D : public ecs::System {
public:
    // Setejem unes variables per gestionar la seq d'animacio del personatge
    static constexpr uint8_t WALK_SEQ[4] = {0, 1, 2, 1};
    static constexpr uint8_t ANIM_SPEED_TICKS = 8;

    // Definim l'index base de cada moviment
    static inline int baseFrame(Facing f) {
        switch (f) {
            case Facing::Down:  return 0;
            case Facing::Up:    return 4;
            case Facing::Right: return 8;
            case Facing::Left:  return 12;
            
        }
        return 0;
    }

    void Update(ecs::ComponentManager& cm) {
        for (auto entity : mEntities) {

        auto& t = cm.GetComponent<TransformComponent>(entity);
        auto& sp = cm.GetComponent<SpriteComponent>(entity);
        
        if (sp.spriteID == 2) { // weapon sprite
            // només mou, no frame
            NF_MoveSprite(1, sp.spriteID, t.x, t.y);
            continue;
        }
        
        int frame = baseFrame(sp.facing);

        if (sp.moving) {
            sp.animTick++;
            if (sp.animTick >= ANIM_SPEED_TICKS) {
                sp.animTick = 0;
                sp.animStep = (sp.animStep + 1) & 0x03; // 0..3
            }
            frame += WALK_SEQ[sp.animStep];
        } else {
            // si está quieto, frame base y reset suave
            sp.animStep = 0;
            sp.animTick = 0;
            frame += 0;
        }

        NF_SpriteFrame(1, sp.spriteID, frame);

        // 2) Posición (enteros)
        NF_MoveSprite(1, sp.spriteID, t.x, t.y);
        }
    }
};
