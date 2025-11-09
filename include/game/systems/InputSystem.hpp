#ifndef GAME_INPUTSYSTEM_HPP
#define GAME_INPUTSYSTEM_HPP
#include "ecs/System.hpp"
#include "ecs/ComponentArray.hpp"
#include "game/Components.hpp"
#include <nds.h>
#include <memory>

class InputSystem : public ecs::System {
public:
    explicit InputSystem(std::shared_ptr<ecs::ComponentArray<InputComponent>> input)
    : input_(std::move(input)) {}

    void update() {
        scanKeys();
        uint32 keys = keysHeld();

        for (const auto& e : mEntities) {
            auto& in = input_->GetData(e);
            in.up    = keys & KEY_UP;
            in.down  = keys & KEY_DOWN;
            in.left  = keys & KEY_LEFT;
            in.right = keys & KEY_RIGHT;
            in.action= keys & KEY_A;
        }
    }
private:
    std::shared_ptr<ecs::ComponentArray<InputComponent>> input_;
};

#endif
