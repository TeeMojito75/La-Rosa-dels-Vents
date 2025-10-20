#include "Board.h"

Board::Board() : turn(0) {
    // Defer sprite creation to init() (requires VRAM resources to be present)
    for (int i = 0; i < 9; ++i) {
        cells[i].ID = i;
        cells[i].X = 0;
        cells[i].Y = 0;
        cells[i].Frame = 0;
    }
}

void Board::init() {
    // Create a 3x3 grid of sprites using the exact same coordinates/steps
    u8 X, Y;
    u8 ID = 0;
    for (X = 55; X < 190; X += 45) {
        for (Y = 32; Y < 167; Y += 45) {
            // screen=1, gfxId=0, palId=0
            NF_CreateSprite(1, ID, 0, 0, X, Y);
            // Start as blank frame (0)
            NF_SpriteFrame(1, ID, 0);

            cells[ID].X = X;
            cells[ID].Y = Y;
            cells[ID].ID = ID;
            cells[ID].Frame = 0;
            ID++;
        }
    }
    turn = 0;
}

bool Board::isInsideCell(int id, int px, int py) const {
    return (px >= cells[id].X && px <= (int)cells[id].X + 32 &&
            py >= cells[id].Y && py <= (int)cells[id].Y + 32);
}

void Board::setCellFrame(int id, u8 frame) {
    cells[id].Frame = frame;
    NF_SpriteFrame(1, id, frame);
}

void Board::handleTouch(const touchPosition& stylus, u16 kDown) {
    if (kDown & KEY_TOUCH) {
        for (int id = 0; id < 9; ++id) {
            if (isInsideCell(id, stylus.px, stylus.py)) {
                if (cells[id].Frame == 0) {
                    if (turn % 2) {
                        // Cross (2) when turn is odd
                        setCellFrame(id, 2);
                    } else {
                        // Circle (1) when turn is even (first move)
                        setCellFrame(id, 1);
                    }
                    // Next turn
                    turn++;
                }
            }
        }
    }
}

void Board::reset() {
    for (int id = 0; id < 9; ++id) {
        setCellFrame(id, 0);
    }
    turn = 0;
}
