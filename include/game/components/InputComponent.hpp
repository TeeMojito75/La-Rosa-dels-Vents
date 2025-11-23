#ifndef INPUT_COMPONENT_HPP
#define INPUT_COMPONENT_HPP

// Plantilla base dels posibles inputs del jugador
struct InputComponent {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

    bool attack = false;
    bool interact = false;
};

#endif
