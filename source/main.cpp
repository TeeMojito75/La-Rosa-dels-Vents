#include <nds.h>
#include <nf_lib.h>

#include "ecs/Entity.hpp"
#include "ecs/EntityManager.hpp"
#include "ecs/ComponentManager.hpp"
#include "ecs/Utils.hpp"

#include "game/components/TransformComponent.hpp"

using namespace ecs;

int main()
{
    consoleDemoInit();
    printf("ECS Test con Transform (Fix 16.16)\n\n");

    // ==== Managers ====
    EntityManager entityManager;
    ComponentManager componentManager;

    // ==== Registrar componente ====
    componentManager.RegisterComponent<TransformComponent>();
    ComponentType transformID = componentManager.GetComponentType<TransformComponent>();

    printf("TransformComponent registrado con ID %u\n\n", transformID);

    // ==== Crear entidades ====
    Entity e1 = entityManager.createEntity();
    Entity e2 = entityManager.createEntity();

    printf("Entidades creadas:\n");
    printf(" e1 id=%u\n", e1.id());
    printf(" e2 id=%u\n\n", e2.id());

    // ==== Firmas ====
    Signature sig;
    sig.reset();
    sig.set(transformID);

    entityManager.setSignature(e1, sig);
    entityManager.setSignature(e2, sig);

    // ==== Crear componentes ====
    TransformComponent t1;
    t1.x = FIX_FROM_INT(10);
    t1.y = FIX_FROM_INT(20);

    TransformComponent t2;
    t2.x = FIX_FROM_INT(50);
    t2.y = FIX_FROM_INT(80);

    componentManager.AddComponent<TransformComponent>(e1, t1);
    componentManager.AddComponent<TransformComponent>(e2, t2);

    // ==== Recuperar los componentes ====
    auto &r1 = componentManager.GetComponent<TransformComponent>(e1);
    auto &r2 = componentManager.GetComponent<TransformComponent>(e2);

    printf("Componentes asignados:\n");
    printf(" e1: x=%d  y=%d\n", FIX_TO_INT(r1.x), FIX_TO_INT(r1.y));
    printf(" e2: x=%d  y=%d\n", FIX_TO_INT(r2.x), FIX_TO_INT(r2.y));

    // ==== Modificar valores ====
    r1.x = r1.x + FIX_FROM_INT(5);
    r1.y = r1.y + FIX_FROM_INT(5);

    printf("\nDespues de modificar e1:\n");
    printf(" e1: x=%d  y=%d\n", FIX_TO_INT(r1.x), FIX_TO_INT(r1.y));

    // ==== Multiplicación de prueba ====
    Fix dobleX = FIX_MUL(r1.x, FIX_FROM_INT(2));
    printf("\nMultiplicacion test (x * 2): %d\n", FIX_TO_INT(dobleX));

    // ==== Loop NDS ====
    while (1)
    {
        swiWaitForVBlank();
    }

    return 0;
}