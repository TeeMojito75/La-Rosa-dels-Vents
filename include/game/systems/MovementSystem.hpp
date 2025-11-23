#ifndef MOVEMENT_SYSTEM_HPP
#define MOVEMENT_SYSTEM_HPP

#include "ecs/System.hpp"
#include "ecs/ComponentManager.hpp"
#include "ecs/Utils.hpp"

#include "game/components/TransformComponent.hpp"
#include "game/components/MovementComponent.hpp"

class MovementSystem : public ecs::System
{
    public:
        void update(ecs::ComponentManager& components);
};

#endif
