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

    int initialX_ = 100;
    int initialY_ = 80;
};
