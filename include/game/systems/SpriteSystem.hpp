#pragma once

#include <nf_lib.h>
#include "ecs/System.hpp"
#include "ecs/ComponentManager.hpp"
#include "game/components/TransformComponent.hpp"
#include "game/components/SpriteComponent.hpp"


class SpriteSystem2D : public ecs::System
{
public:
    void Update(ecs::ComponentManager& cm)
    {
        for (auto e : mEntities)
        {
            auto& t  = cm.GetComponent<TransformComponent>(e);
            auto& sp = cm.GetComponent<SpriteComponent>(e);

            NF_MoveSprite(1, sp.spriteID, t.x, t.y);
        }
    }
};
