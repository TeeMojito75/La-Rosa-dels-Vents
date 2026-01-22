#pragma once
#include <cstdint>
#include <cstdio>
#include <cctype>

class CollisionMap {
public:
    static constexpr int W = 16;
    static constexpr int H = 16;

    // Carga un CSV de Tiled donde:
    // - 0  = vacío (no sólido)
    // - >0 = tile pintado (sólido)
    bool loadCSV(const char* path) {
        FILE* f = std::fopen(path, "rb");
        if (!f) return false;

        int idx = 0;
        int value = 0;
        bool inNumber = false;
        bool negative = false;

        int ch;
        while ((ch = std::fgetc(f)) != EOF) {
            if (ch == '-') { // por si acaso, no debería aparecer en gids
                negative = true;
                continue;
            }

            if (std::isdigit(ch)) {
                inNumber = true;
                value = value * 10 + (ch - '0');
            } else {
                // separador (coma, salto de línea, espacios, etc.)
                if (inNumber) {
                    if (negative) value = -value;

                    if (idx < W * H) {
                        data_[idx++] = (value != 0) ? 1 : 0;
                    }
                    // reset
                    value = 0;
                    inNumber = false;
                    negative = false;
                }
            }

            if (idx >= W * H) break; // ya tenemos 256 celdas
        }

        // Si el archivo acaba sin separador, finalizamos último número
        if (inNumber && idx < W * H) {
            if (negative) value = -value;
            data_[idx++] = (value != 0) ? 1 : 0;
        }

        std::fclose(f);
        return idx == W * H;
    }

    bool isSolidTile(int tx, int ty) const {
        if (tx < 0 || ty < 0 || tx >= W || ty >= H) return true; // fuera = pared
        return data_[ty * W + tx] != 0;
    }

    bool isSolidPixel(int x, int y) const {
        return isSolidTile(x >> 4, y >> 4); // /16
    }

private:
    uint8_t data_[W * H]{};
};
