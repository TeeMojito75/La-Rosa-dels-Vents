#pragma once
#include <nds.h>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include "ecs/System.hpp"
#include "ecs/ComponentArray.hpp"
#include "ecs/Entity.hpp"
#include "game/Components.hpp"
#include "game/Services.hpp"

class DebugDrawSystem : public ecs::System {
public:
    DebugDrawSystem(std::shared_ptr<ecs::ComponentArray<Transform2DComponent>> pos,
                    const TilemapService* tiles,
                    int mapW, int mapH, int tileSize)
    : pos_(std::move(pos)), tiles_(tiles), W_(mapW), H_(mapH), TS_(tileSize)
    {
        // Pintar mapa estático (# = sólido, ' ' = vacío)
        consoleClear();
        for (int y = 0; y < H_; ++y) {
            for (int x = 0; x < W_; ++x) {
                bool solid = tiles_->isSolidTile(x, y);
                putcAt(x, y, solid ? '#' : ' ');
            }
        }
        // Al borrar la posición anterior:
        putcAt(0, H_, 0);
        printf("D-Pad: mover, A: (sin uso)");
    }

    void update()
    {
        // Borra la marca anterior y pinta la nueva para cada entidad
        for (const auto& e : mEntities) {
            auto& p = pos_->GetData(e);

            int cx = p.x / TS_;
            int cy = p.y / TS_;
            cx = std::clamp(cx, 0, W_ - 1);
            cy = std::clamp(cy, 0, H_ - 1);


            auto it = lastPos_.find(e.id());
            if (it != lastPos_.end()) {
                // repinta fondo (tile sólido/espacio) en la posición anterior
                int px = it->second.first, py = it->second.second;
                bool solid = tiles_->isSolidTile(px, py);
                putcAt(px, py, solid ? '#' : ' ');
            }
            // pinta jugador
            putcAt(cx, cy, 'O');
            lastPos_[e.id()] = {cx, cy};
        }
    }

private:
    static inline void putcAt(int col, int row, char ch)
    {
        // Mover cursor: \x1b[row;colH   (1-based)
        printf("\x1b[%d;%dH", row + 1, col + 1);
        if (ch) printf("%c", ch);
    }

    std::shared_ptr<ecs::ComponentArray<Transform2DComponent>> pos_;
    const TilemapService* tiles_;
    int W_, H_, TS_;

    std::unordered_map<ecs::EntityId, std::pair<int,int>> lastPos_;
};
