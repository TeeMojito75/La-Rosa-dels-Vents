#ifndef GAME_MOVEMENTSYSTEM_HPP
#define GAME_MOVEMENTSYSTEM_HPP
#include <memory>
#include "ecs/System.hpp"
#include "ecs/ComponentArray.hpp"
#include "game/Components.hpp"
#include <memory>

class MovementSystem : public ecs::System {
    public:
        MovementSystem(std::shared_ptr<ecs::ComponentArray<Transform2DComponent>> pos,
                    std::shared_ptr<ecs::ComponentArray<MovementComponent>> mov)
        : pos_(std::move(pos)), mov_(std::move(mov)) {}

        void update() {
            for (const auto& e : mEntities) {
                auto& p = pos_->GetData(e);
                auto& m = mov_->GetData(e);
                p.x += m.vx;
                p.y += m.vy;
            }
        }

    private:
        std::shared_ptr<ecs::ComponentArray<Transform2DComponent>> pos_;
        std::shared_ptr<ecs::ComponentArray<MovementComponent>> mov_;
};

#endif