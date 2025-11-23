#ifndef INPUT_SYSTEM_HPP
#define INPUT_SYSTEM_HPP

#include "ecs/System.hpp"
#include "ecs/ComponentManager.hpp"
#include "game/components/InputComponent.hpp"

class InputSystem : public ecs::System
{
    public:
        void update(ecs::ComponentManager& components);
};

#endif
