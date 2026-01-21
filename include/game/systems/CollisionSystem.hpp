#pragma once
#include "ecs/System.hpp"
#include "ecs/ComponentManager.hpp"

#include "game/components/TransformComponent.hpp"
#include "game/components/VelocityComponent.hpp"
#include "game/components/CollisionComponent.hpp"
#include "game/inits/CollisionMap.hpp"

// game/systems/CollisionSystem.hpp
class CollisionSystem : public ecs::System {
public:
    CollisionSystem() = default;

    void Init(const CollisionMap* map) { map_ = map; }

    void Update(ecs::ComponentManager& cm) {
        if (!map_) return;

        for (ecs::Entity e : mEntities) {
            auto& t = cm.GetComponent<TransformComponent>(e);
            auto& v = cm.GetComponent<VelocityComponent>(e);
            auto& c = cm.GetComponent<CollisionComponent>(e);
            if (!c.solid) continue;

            // Clamp per eixos (NO moguis Transform aquí si MovementSystem ja mou)
            if (collidesAt(t.x + v.vx, t.y, c.w, c.h)) v.vx = 0;
            if (collidesAt(t.x, t.y + v.vy, c.w, c.h)) v.vy = 0;

        }
    }

private:
    const CollisionMap* map_ = nullptr;

    bool collidesAt(int x, int y, int w, int h) const {
        int left = x;
        int right = x + w - 1;
        int top = y;
        int bottom = y + h - 1;

        return map_->isSolidPixel(left, top) ||
               map_->isSolidPixel(right, top) ||
               map_->isSolidPixel(left, bottom) ||
               map_->isSolidPixel(right, bottom);
    }
};

