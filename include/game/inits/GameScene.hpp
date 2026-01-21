#pragma once

#include <memory>

#include "ecs/EntityManager.hpp"
#include "ecs/ComponentManager.hpp"
#include "ecs/SystemManager.hpp"
#include "ecs/Utils.hpp"

#include "game/inits/BackgroundRenderer.hpp"

// Components
#include "game/components/TransformComponent.hpp"
#include "game/components/VelocityComponent.hpp"
#include "game/components/SpriteComponent.hpp"
// (futuros)
//#include "game/components/CollisionComponent.hpp"
//#include "game/components/HealthComponent.hpp"

// Systems
#include "game/systems/PlayerInputSystem.hpp"
#include "game/systems/MovementSystem.hpp"
#include "game/systems/SpriteSystem.hpp"
// (futuros)
#include "game/systems/CollisionSystem.hpp"
//#include "game/systems/CombatSystem.hpp"

#include "game/inits/CollisionMap.hpp"


class GameScene {
public:
    explicit GameScene(BackgroundRenderer& renderer);

    bool init();     // init graphics + init ECS + crear entidades demo
    void run();      // game loop

private:
    void initECS();
    void createDemoEntities();

private:
    BackgroundRenderer& renderer_;

    ecs::EntityManager    em_;
    ecs::ComponentManager cm_;
    ecs::SystemManager    sm_;

    // Sistemas (punteros devueltos por RegisterSystem)
    std::shared_ptr<PlayerInputSystem> inputSys_;
    std::shared_ptr<MovementSystem>    moveSys_;
    std::shared_ptr<SpriteSystem2D>    spriteSys_;

    // Futuros (si aún no existen, déjalos comentados o como nullptr)
    std::shared_ptr<CollisionSystem>   collisionSys_;
    //std::shared_ptr<CombatSystem>      combatSys_;

    ecs::Entity player_;
    bool running_ = true;
    CollisionMap collisionMap_;
};
