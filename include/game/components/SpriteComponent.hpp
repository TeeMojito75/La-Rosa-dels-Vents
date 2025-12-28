#pragma once

// Definim una estructura per determinar la direcció de moviment
enum class Facing : uint8_t { Down=0, Up=1, Left=2, Right=3 };

struct SpriteComponent {
    int spriteID;   // índice del sprite 2D
    int gfxID;      // slot de VRAM del gráfico
    int palID;      // slot de paleta

    Facing facing = Facing::Down; // Dirrecció mira personatge
    bool moving = false; // Identificador moviment actual

    uint8_t animStep = 0;     // 0..3
    uint8_t animTick = 0;     // contador
};
