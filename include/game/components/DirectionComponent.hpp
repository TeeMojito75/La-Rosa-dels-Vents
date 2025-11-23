#ifndef DIRECTION_COMPONENT_HPP
#define DIRECTION_COMPONENT_HPP

#include <cstdint>

enum class Direction : uint8_t {
    Down = 0,
    Up,
    Left,
    Right
};

struct DirectionComponent {
    Direction dir = Direction::Down;
};

#endif
