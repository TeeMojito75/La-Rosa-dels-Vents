#include "Game.h"
#include <stdio.h>


void Game::initFs() {
    if (!nitroFSInit(NULL)) { while (1) swiWaitForVBlank(); }
    chdir("nitro:/");
    NF_SetRootFolder("NITROFS"); 
}

void Game::initVideo() {
    // 2D mode on both screens
    NF_Set2D(0, 0);
    NF_Set2D(1, 0);

    // Init background buffers/systems
    NF_InitTiledBgBuffers();
    NF_InitTiledBgSys(0);
    NF_InitTiledBgSys(1);

    // Init sprite buffers/system for bottom screen
    NF_InitSpriteBuffers();
    NF_InitSpriteSys(1);
}

void Game::loadResources() {
    // Fondos 256x256
    NF_LoadTiledBg("Backgrounds/Top/Top", "Top",    256, 256);
    NF_LoadTiledBg("Backgrounds/Bottom/Bottom", "Bottom", 256, 256);

    // Sprites 32x32
    NF_LoadSpriteGfx("Sprites/TicTacToe", 0, 32, 32);
    NF_LoadSpritePal("Sprites/TicTacToe", 0);


    // Move to VRAM
    NF_VramSpriteGfx(1, 0, 0, false);
    NF_VramSpritePal(1, 0, 0);
}

void Game::createBackgrounds() {
    // Create tiled backgrounds
    NF_CreateTiledBg(0, 3, "Top");
    NF_CreateTiledBg(1, 3, "Bottom");
}

void Game::run() {
 
    initFs();
    initVideo();
    loadResources();
    createBackgrounds();

    // Board (grid + turn logic)
    Board board;
    board.init();

    touchPosition stylus;

    while (1) {
        // Input
        scanKeys();
        touchRead(&stylus);
        u16 kDown = keysDown();

        // Touch-to-place logic
        board.handleTouch(stylus, kDown);

        // Reset with A (exactly same behavior as original)
        if (kDown & KEY_A) {
            board.reset();
        }

        // OAM updates
        NF_SpriteOamSet(1);
        swiWaitForVBlank();
        oamUpdate(&oamSub);
    }
}
