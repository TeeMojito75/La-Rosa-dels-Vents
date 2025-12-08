#include <nds.h>
#include <nf_lib.h>
#include <filesystem.h>

#include "ecs/EntityManager.hpp"
#include "ecs/ComponentManager.hpp"
#include "ecs/SystemManager.hpp"

#include "ecs/Utils.hpp"

#include "game/components/TransformComponent.hpp"
#include "game/components/VelocityComponent.hpp"
#include "game/components/SpriteComponent.hpp"

#include "game/systems/MovementSystem.hpp"
#include "game/systems/PlayerInputSystem.hpp"
#include "game/systems/SpriteSystem.hpp"

using namespace ecs;

int main() {
    consoleDemoInit();

    // NitroFS
    if (!nitroFSInit(NULL)) { while (1) swiWaitForVBlank(); } 
    chdir("nitro:/"); 
    NF_SetRootFolder("NITROFS");

    // Modo 2D en ambas pantallas
    NF_Set2D(0, 0);
    NF_Set2D(1, 0);

    NF_InitTiledBgBuffers();
    NF_InitTiledBgSys(1);

    NF_InitSpriteBuffers();
    NF_InitSpriteSys(1);

    // Fondos 256x256
    NF_LoadTiledBg("Backgrounds/Bottom/Bottom", "Bottom", 256, 256);
    
    // Cargar sprite
    NF_LoadSpriteGfx("Sprites/Jugador/Pages", 0, 16, 16);
    NF_LoadSpritePal("Sprites/Jugador/Pages", 0);

    // Copiar a VRAM pantalla inferior
    NF_VramSpriteGfx(1, 0, 0, false);
    NF_VramSpritePal(1, 0, 0);

    // Crear sprite en VRAM
    NF_CreateTiledBg(1, 3, "Bottom");
    NF_CreateSprite(1, 0, 0, 0, 100, 80); // screen=1, id=0
    NF_SpriteFrame(1, 0, 0);

    // ================== ECS SETUP ==================
    EntityManager em;
    ComponentManager cm;
    SystemManager sm;

    cm.RegisterComponent<TransformComponent>();
    cm.RegisterComponent<VelocityComponent>();
    cm.RegisterComponent<SpriteComponent>();

    auto moveSys   = sm.RegisterSystem<MovementSystem>();
    auto inputSys  = sm.RegisterSystem<PlayerInputSystem>();
    auto spriteSys = sm.RegisterSystem<SpriteSystem2D>();

    ComponentType tid = cm.GetComponentType<TransformComponent>();
    ComponentType vid = cm.GetComponentType<VelocityComponent>();
    ComponentType sid = cm.GetComponentType<SpriteComponent>();

    // Movement
    Signature ms; ms.set(tid); ms.set(vid);
    sm.SetSignature<MovementSystem>(ms);

    // Input
    Signature is; is.set(vid);
    sm.SetSignature<PlayerInputSystem>(is);

    // Sprites
    Signature ss; ss.set(tid); ss.set(sid);
    sm.SetSignature<SpriteSystem2D>(ss);

    // Crear entidad jugador
    Entity p = em.createEntity();

    Signature ps; ps.set(tid); ps.set(vid); ps.set(sid);
    em.setSignature(p, ps);
    sm.EntitySignatureChanged(p, ps);

    TransformComponent t{ 100, 80 };
    VelocityComponent v{ 0, 0 };
    SpriteComponent sp{ 0, 0, 0 };

    cm.AddComponent<TransformComponent>(p, t);
    cm.AddComponent<VelocityComponent>(p, v);
    cm.AddComponent<SpriteComponent>(p, sp);

    // ================ LOOP =================
    while (1) {
        inputSys->Update(cm);     // Cambia velocidad
        moveSys->Update(cm);      // Cambia posición (solo una vez)
        spriteSys->Update(cm);    // Aplica posición al sprite (solo una vez)

        NF_SpriteOamSet(1);       // Copia shadow OAM → real OAM
        swiWaitForVBlank();       // Esperar VBlank
        oamUpdate(&oamSub);       // Actualizar hardware OAM UNA vez
    }


    return 0;
}
