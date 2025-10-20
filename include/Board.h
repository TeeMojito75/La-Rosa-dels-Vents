#pragma once

#include <nds.h>
#include <nf_lib.h>

// Simple cell data (kept equivalent to original struct)
typedef struct {
    u8 ID;
    u8 X;
    u8 Y;
    u8 Frame;
} SpriteCell;

// Manages the 3x3 sprite grid and turn logic
class Board {
public:
    Board();
    // Initialize sprites and cell data. Must be called after VRAM resources are loaded.
    void init();
    // Handle a single input tick (touch-to-place). Does not block.
    void handleTouch(const touchPosition& stylus, u16 keysDown);
    // Clear board to blank frames and reset the turn counter.
    void reset();
private:
    SpriteCell cells[9];
    u8 turn; // same semantics as original: 0 => first move is circle, odd => cross
    // Helpers
    bool isInsideCell(int id, int px, int py) const;
    void setCellFrame(int id, u8 frame);
};
