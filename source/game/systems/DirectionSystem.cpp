#include "game/systems/DirectionSystem.hpp"

using namespace ecs;

void DirectionSystem::update(ComponentManager& components)
{
    const Fix speed = FIX_FROM_INT(1);

    for (Entity e : mEntities)
    {
        auto& input = components.GetComponent<InputComponent>(e);
        auto& movement = components.GetComponent<MovementComponent>(e);
        auto& direction = components.GetComponent<DirectionComponent>(e);

        movement.vx = 0;
        movement.vy = 0;

        if (input.up) 
        {
            movement.vy = -speed;
            direction.dir = Direction::Up;
        } 
        else if (input.down) 
        {
            movement.vy = speed;
            direction.dir = Direction::Down;
        }

        if (input.left)
        {
            movement.vx = -speed;
            direction.dir = Direction::Left;
        } 
        else if (input.right) 
        {
            movement.vx = speed;
            direction.dir = Direction::Right;
        }
    }
}
