#ifndef DIRECTION_SYSTEM_HPP
#define DIRECTION_SYSTEM_HPP

#include "ecs/System.hpp"
#include "ecs/ComponentManager.hpp"
#include "ecs/Utils.hpp"

#include "game/components/InputComponent.hpp"
#include "game/components/MovementComponent.hpp"
#include "game/components/DirectionComponent.hpp"

class DirectionSystem : public ecs::System
{
    public:
        void update(ecs::ComponentManager& components);
};

#endif
