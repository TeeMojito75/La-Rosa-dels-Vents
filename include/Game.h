#pragma once

#include <nds.h>
#include <nf_lib.h>
#include <filesystem.h>
#include <sys/unistd.h>
#include "Board.h"

// Bootstraps the DS/NF systems, loads resources, and runs the main loop
class Game {
    
    public:
        Game() = default;
        void run();

    private:
        void initFs();
        void initVideo();
        void loadResources();
        void createBackgrounds();
};
