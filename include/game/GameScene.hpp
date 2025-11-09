#ifndef GAME_GAMESCENE_HPP
#define GAME_GAMESCENE_HPP
#include <memory>
#include "ecs/EntityManager.hpp"
#include "ecs/ComponentManager.hpp"
#include "ecs/SystemManager.hpp"
#include "game/Components.hpp"
#include "game/Services.hpp"
#include "game/systems/InputSystem.hpp"
#include "game/systems/MovementSystem.hpp"
#include "game/systems/CollisionSystem.hpp"
// Temp
#include "game/systems/DebugDrawSystem.hpp" 

class GameScene {
public:
    GameScene(const uint8_t* tilemap, int tw, int th, int ts)
    : tiles(tilemap, tw, th, ts) {
        cm.RegisterComponent<Transform2DComponent>();
        cm.RegisterComponent<MovementComponent>();
        cm.RegisterComponent<CollisionComponent>();
        cm.RegisterComponent<InputComponent>();

        auto pos = cm.GetComponentArray<Transform2DComponent>();
        auto mov = cm.GetComponentArray<MovementComponent>();
        auto col = cm.GetComponentArray<CollisionComponent>();
        auto inp = cm.GetComponentArray<InputComponent>();

        inputSys = sm.RegisterSystem<InputSystem>(inp);
        moveSys  = sm.RegisterSystem<MovementSystem>(pos, mov);
        collSys  = sm.RegisterSystem<CollisionSystem>(pos, mov, col, &tiles);
        debugSys = sm.RegisterSystem<DebugDrawSystem>(pos, &tiles, tw, th, ts);   // NUEVO

        ecs::Signature sigIn;  sigIn.reset();  sigIn.set(cm.GetComponentType<InputComponent>());
        ecs::Signature sigMov; sigMov.reset(); sigMov.set(cm.GetComponentType<Transform2DComponent>()); sigMov.set(cm.GetComponentType<MovementComponent>());
        ecs::Signature sigCol; sigCol.reset(); sigCol.set(cm.GetComponentType<Transform2DComponent>()); sigCol.set(cm.GetComponentType<MovementComponent>()); sigCol.set(cm.GetComponentType<CollisionComponent>());
        ecs::Signature sigDbg; sigDbg.reset(); sigDbg.set(cm.GetComponentType<Transform2DComponent>()); // NUEVO

        sm.SetSignature<InputSystem>(sigIn);
        sm.SetSignature<MovementSystem>(sigMov);
        sm.SetSignature<CollisionSystem>(sigCol);
        sm.SetSignature<DebugDrawSystem>(sigDbg); // NUEVO

        // Player
        player = em.createEntity();
        cm.AddComponent<Transform2DComponent>(player, {20, 20});
        cm.AddComponent<MovementComponent>(player, {});
        cm.AddComponent<CollisionComponent>(player, {});
        cm.AddComponent<InputComponent>(player, {});

        ecs::Signature eSig; eSig.reset();
        eSig.set(cm.GetComponentType<Transform2DComponent>());
        eSig.set(cm.GetComponentType<MovementComponent>());
        eSig.set(cm.GetComponentType<CollisionComponent>());
        eSig.set(cm.GetComponentType<InputComponent>());
        em.setSignature(player, eSig);
        sm.EntitySignatureChanged(player, eSig);
    }

    void update() {
        inputSys->update();
        propagateInputToMovement();
        collSys->update();   // ← ahora primero
        moveSys->update();   // ← después aplicamos vx/vy ya validados
        debugSys->update();
    }

private:
    void propagateInputToMovement() {
        auto& in  = cm.GetComponent<InputComponent>(player);
        auto& mov = cm.GetComponent<MovementComponent>(player);
        const int speed = 1;
        mov.vx = (in.right - in.left) * speed;
        mov.vy = (in.down  - in.up)   * speed;
    }

private:
    ecs::EntityManager em;
    ecs::ComponentManager cm;
    ecs::SystemManager sm;
    TilemapService tiles;

    ecs::Entity player;

    std::shared_ptr<InputSystem>     inputSys;
    std::shared_ptr<MovementSystem>  moveSys;
    std::shared_ptr<CollisionSystem> collSys;
    std::shared_ptr<DebugDrawSystem> debugSys; // NUEVO
};
#endif