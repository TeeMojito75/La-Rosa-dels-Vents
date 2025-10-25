#include <nds.h>
#include <stdio.h>
#include "ECS/EntityManager.hpp"

using namespace ecs;

int main() {
    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);
    consoleDemoInit();

    EntityManager entityMan;

    // Creació 5 entitats i mostram les ids
    printf("Creant 5 entitats...\n");

    Entity e[5];
    for (int i = 0; i < 5; i++) {
        e[i] = entityMan.create();
        printf("e[%d] = %d\n", i, (unsigned)e[i]);
    }

    // Destruim 2 i tornem a crear 2, per veure entitatsLLiures funcionant
    printf("Destruint e[1] i e[3]\n");

    entityMan.destroy(e[1]);
    entityMan.destroy(e[3]);
    printf("Entitats vives: %u\n", (unsigned)entityMan.size());

    printf("Creando 2 entidades nuevas...\n");
    Entity a = entityMan.create();
    Entity b = entityMan.create();
    printf("a=%u b=%u", (unsigned)a, (unsigned)b);
    printf("Vives: %u", (unsigned)entityMan.size());


    printf("Paso 1 OK. Pulsa START para reiniciar.\n");
    while (1) {
        scanKeys();
        if (keysDown() & KEY_START) break;
        swiWaitForVBlank();
    }
    return 0;
}