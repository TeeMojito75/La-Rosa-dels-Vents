#pragma once
#include <memory>
#include "ecs/System.hpp"
#include "ecs/ComponentArray.hpp"
#include "game/Components.hpp"
#include "game/Services.hpp"

class CollisionSystem : public ecs::System {
public:
    CollisionSystem(std::shared_ptr<ecs::ComponentArray<Transform2DComponent>> pos,
                    std::shared_ptr<ecs::ComponentArray<MovementComponent>>    mov,
                    std::shared_ptr<ecs::ComponentArray<CollisionComponent>>   col,
                    const TilemapService* tiles)
    : pos_(std::move(pos)), mov_(std::move(mov)), col_(std::move(col)), tiles_(tiles) {}

    // Orden recomendado en GameScene::update():
    // input -> propagateInputToMovement -> collision -> movement -> debugDraw
    void update() {
        for (const auto& e : mEntities) {
            auto& p = pos_->GetData(e);
            auto& m = mov_->GetData(e);
            auto& c = col_->GetData(e);

            // Eje X: si el siguiente X chocaría, anula vx
            if (m.vx != 0) {
                const int nx = p.x + m.vx;
                if (collidesAABB_TILES_(nx, p.y, c)) {
                    m.vx = 0;
                }
            }
            // Eje Y: usa X ya resuelto (por si quedó a 0 arriba)
            if (m.vy != 0) {
                const int nx = p.x + m.vx;
                const int ny = p.y + m.vy;
                if (collidesAABB_TILES_(nx, ny, c)) {
                    m.vy = 0;
                }
            }
        }
    }

private:
    // Comprueba si el AABB centrado en (x,y) toca algún tile sólido (rango completo de tiles).
    bool collidesAABB_TILES_(int x, int y, const CollisionComponent& c) const {
        const int ts = tiles_->tileSize();

        // AABB en píxeles (top-left inclusivo / bottom-right inclusivo)
        const int left   = x + c.offsetX - c.halfW;
        const int right  = x + c.offsetX + c.halfW - 1;
        const int top    = y + c.offsetY - c.halfH;
        const int bottom = y + c.offsetY + c.halfH - 1;

        // Convertir a rango de tiles
        const int minTx = left   / ts;
        const int maxTx = right  / ts;
        const int minTy = top    / ts;
        const int maxTy = bottom / ts;

        // Recorre TODOS los tiles tocados
        for (int ty = minTy; ty <= maxTy; ++ty) {
            for (int tx = minTx; tx <= maxTx; ++tx) {
                if (tiles_->isSolidTile(tx, ty)) return true;
            }
        }
        return false;
    }

    std::shared_ptr<ecs::ComponentArray<Transform2DComponent>> pos_;
    std::shared_ptr<ecs::ComponentArray<MovementComponent>>    mov_;
    std::shared_ptr<ecs::ComponentArray<CollisionComponent>>   col_;
    const TilemapService* tiles_;
};
