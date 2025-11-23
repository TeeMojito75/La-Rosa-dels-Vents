#ifndef ANIMATION_COMPONENT_HPP
#define ANIMATION_COMPONENT_HPP

// Plantilla temporal
struct AnimationComponent {
    int firstFrame = 0;
    int lastFrame = 0;
    int currentFrame = 0;

    int frameDelay = 8;
    int timer = 0;
};

#endif
