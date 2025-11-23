#include "game/systems/MovementSystem.hpp"

using namespace ecs;

void MovementSystem::update(ComponentManager& components)
{
    for (Entity e : mEntities)
    {
        auto& transform = components.GetComponent<TransformComponent>(e);
        auto& movement = components.GetComponent<MovementComponent>(e);

        transform.x += movement.vx;
        transform.y += movement.vy;
    }
}
