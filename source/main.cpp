#include <nds.h>
#include <nf_lib.h>

#include "ecs/EntityManager.hpp"
#include "ecs/ComponentManager.hpp"
#include "ecs/SystemManager.hpp"
#include "ecs/Utils.hpp"

#include "game/components/TransformComponent.hpp"
#include "game/components/VelocityComponent.hpp"

#include "game/systems/MovementSystem.hpp"
#include "game/systems/PlayerInputSystem.hpp"

using namespace ecs;

int main()
{
    consoleDemoInit();

    EntityManager em;
    ComponentManager cm;
    SystemManager sm;

    // Registrar componentes
    cm.RegisterComponent<TransformComponent>();
    cm.RegisterComponent<VelocityComponent>();

    ComponentType tID = cm.GetComponentType<TransformComponent>();
    ComponentType vID = cm.GetComponentType<VelocityComponent>();

    // Registrar sistemas
    auto moveSys  = sm.RegisterSystem<MovementSystem>();
    auto inputSys = sm.RegisterSystem<PlayerInputSystem>();

    // Firmas
    Signature moveSig;
    moveSig.set(tID);
    moveSig.set(vID);
    sm.SetSignature<MovementSystem>(moveSig);

    Signature inputSig;
    inputSig.set(vID);
    sm.SetSignature<PlayerInputSystem>(inputSig);

    // Crear player
    Entity player = em.createEntity();

    Signature playerSig;
    playerSig.set(tID);
    playerSig.set(vID);
    em.setSignature(player, playerSig);
    sm.EntitySignatureChanged(player, playerSig);

    // Añadir componentes
    cm.AddComponent<TransformComponent>(player, { FIX_FROM_INT(30), FIX_FROM_INT(30) });
    cm.AddComponent<VelocityComponent>(player, { FIX_ZERO, FIX_ZERO });

    // Bucle principal
    while (1) {
        inputSys->Update(cm);
        moveSys->Update(cm);

        auto& pos = cm.GetComponent<TransformComponent>(player);
        auto& vel = cm.GetComponent<VelocityComponent>(player);

        consoleClear();
        printf("Pos: %d, %d\n", FIX_TO_INT(pos.x), FIX_TO_INT(pos.y));
        printf("Vel: %d, %d\n", FIX_TO_INT(vel.vx), FIX_TO_INT(vel.vy));

        swiWaitForVBlank();
    }

    return 0;
}
