#include "game/inits/BackgroundRenderer.hpp"

bool BackgroundRenderer::init() {
    consoleDemoInit();

    // NitroFS
    if (!nitroFSInit(NULL)) return false;
    chdir("nitro:/");
    NF_SetRootFolder("NITROFS");

    // Modo 2D en ambas pantallas
    NF_Set2D(0, 0);
    NF_Set2D(1, 0);

    // Backgrounds (tilemaps)
    NF_InitTiledBgBuffers();
    NF_InitTiledBgSys((int)Screen::Sub);

    // Sprites
    NF_InitSpriteBuffers();
    NF_InitSpriteSys((int)Screen::Sub);

    return true;
}

void BackgroundRenderer::loadResources() {
    // Fondos 256x256
    NF_LoadTiledBg("Backgrounds/Bottom/Bottom_final", "Bottom", 256, 256);
    NF_LoadTiledBg("Backgrounds/Bottom/Decor_compact", "Decor", 256, 256);

    // Sprite jugador 16x16 (slot 0)
    NF_LoadSpriteGfx("Sprites/Jugador/Pages", spriteGfxId_, 16, 16);
    NF_LoadSpritePal("Sprites/Jugador/Pages", spritePalId_);

    // Sprite enemic 16x16 (slot 0)
    NF_LoadSpriteGfx("Sprites/Enemic/Enemic_sharedpal", enemyGfxId_, 16, 16);
    NF_LoadSpritePal("Sprites/Enemic/Enemic1", enemyPalId_);

    // Sprite arma 16x16 (slot 0)
    NF_LoadSpriteGfx("Sprites/Arma/Arma", weaponGfxId_, 16, 16);
    NF_LoadSpritePal("Sprites/Arma/Arma", weaponPalId_);
}

void BackgroundRenderer::setupScene() {
    const int scr = (int)screen_;

    // Copiar jugador a VRAM (pantalla inferior)
    NF_VramSpriteGfx(scr, spriteGfxId_, spriteGfxId_, false);
    NF_VramSpritePal(scr, spritePalId_, spritePalId_);

    // Copiar enemic a VRAM (pantalla inferior)
    NF_VramSpriteGfx(scr, enemyGfxId_, enemyGfxId_, false);
    NF_VramSpritePal(scr, enemyPalId_, enemyPalId_);
    
    // Copiar arma a VRAM (pantalla inferior)
    NF_VramSpriteGfx(scr, weaponGfxId_, weaponGfxId_, false);
    NF_VramSpritePal(scr, weaponPalId_, weaponPalId_);

    // Crear fondos (ejemplo: capa 3 y 2)
    NF_CreateTiledBg(scr, (int)Layer::L3, "Bottom");
    NF_CreateTiledBg(scr, (int)Layer::L2, "Decor");

    // Crear sprite jugador en VRAM
    NF_CreateSprite(scr, spriteId_, spriteGfxId_, spritePalId_, initialPX_, initialPY_);
    NF_SpriteLayer(scr, spriteId_, 0);
    NF_SpriteFrame(scr, spriteId_, 0);

    // Crear sprite enemic en VRAM
    NF_CreateSprite(scr, enemySpriteId_, enemyGfxId_, 1, initialEX_, initialEY_);
    NF_SpriteLayer(scr, enemySpriteId_, 1);
    NF_SpriteFrame(scr, enemySpriteId_, 1);

    // Crear sprite arma VRAM
    NF_CreateSprite(scr, weaponSpriteId_, weaponGfxId_, weaponPalId_, 256, 192);
    NF_SpriteLayer(scr, weaponSpriteId_, 0);
    NF_SpriteFrame(scr, weaponSpriteId_, 0);
}

void BackgroundRenderer::commitOAMSub() {
    // Empaqueta el estado de sprites para la sub-screen
    NF_SpriteOamSet((int)Screen::Sub);

    // Sincroniza con VBlank y aplica a OAM sub
    swiWaitForVBlank();
    oamUpdate(&oamSub);
}
