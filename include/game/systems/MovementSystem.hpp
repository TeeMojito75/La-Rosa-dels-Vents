#ifndef GAME_SYSTEMS_MOVEMENTSYSTEM_HPP
#define GAME_SYSTEMS_MOVEMENTSYSTEM_HPP

#include <unordered_set>
#include "ecs/ComponentManager.hpp"
#include "ecs/System.hpp"
#include "game/components/TransformComponent.hpp"
#include "game/components/VelocityComponent.hpp"


class MovementSystem : public ecs::System {
public:
    void Update(ecs::ComponentManager& cm) {
        for (ecs::Entity e : mEntities) {

            auto& t = cm.GetComponent<TransformComponent>(e);
            auto& v = cm.GetComponent<VelocityComponent>(e);

            t.x += v.vx;
            t.y += v.vy;
        }
    }
};

#endif