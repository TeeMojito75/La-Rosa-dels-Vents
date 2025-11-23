#include <nds.h>

int main() {
    consoleDemoInit();

    while (1) {
        swiWaitForVBlank();
    }
    return 0;
}
