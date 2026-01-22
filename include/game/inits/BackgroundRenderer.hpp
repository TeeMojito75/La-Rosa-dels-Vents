#pragma once

#include <cstdint>
#include <string>

#include <nds.h>
#include <filesystem.h>
#include <nf_lib.h>

enum class Screen : int { Main = 0, Sub = 1 };
enum class Layer  : int { L0 = 0, L1 = 1, L2 = 2, L3 = 3 };

class BackgroundRenderer {
public:
    BackgroundRenderer() = default;

    bool init();

    void loadResources();

    void setupScene();

    void commitOAMSub(); 

private:

    Screen screen_ = Screen::Sub;

    int spriteGfxId_ = 0;
    int spritePalId_ = 0;
    int spriteId_    = 0;

    int enemyGfxId_  = 1;
    int enemyPalId_  = 1;
    int enemySpriteId_ = 1;

    int weaponGfxId_    = 2;
    int weaponPalId_    = 2;
    int weaponSpriteId_ = 2;


    int initialPX_ = 100;
    int initialPY_ = 80;

    int initialEX_ = 200;
    int initialEY_ = 80;
};
