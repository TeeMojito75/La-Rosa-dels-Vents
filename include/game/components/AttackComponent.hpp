#pragma once

struct AttackComponent {
    int damage = 1;
    int ttlFrames = 10;   // quant dura l’atac
    bool fromPlayer = true;
    bool active = false;
};
