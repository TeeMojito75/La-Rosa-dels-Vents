#ifndef GAME_COMPONENTS_HPP
#define GAME_COMPONENTS_HPP
#include <cstdint>

struct Transform2DComponent 
{
    int x = 0, y = 0;
};

struct MovementComponent 
{
    int vx = 0, vy = 0;
};

struct CollisionComponent 
{    
    int halfW = 8, halfH = 8;
    int offsetX = 0, offsetY = 0;
    bool solid = true;
};

struct InputComponent 
{
    bool up = false, down = false, left = false, right = false;
    bool action = false;
};

#endif