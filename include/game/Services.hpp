#ifndef GAME_SERVICES_HPP
#define GAME_SERVICES_HPP

#include <cstdint>

// Services.hpp
class TilemapService {
public:
    TilemapService(const uint8_t* data, int width, int height, int tileSize)
    : data_(data), w_(width), h_(height), ts_(tileSize) {}

    // NUEVO: consulta directa por tile
    bool isSolidTile(int tx, int ty) const {
        if (tx < 0 || ty < 0 || tx >= w_ || ty >= h_) return true; // fuera: sólido
        return data_[ty * w_ + tx] != 0;
    }

    // Si quieres mantener la versión por píxel:
    bool isSolidPx(int px, int py) const {
        int tx = px / ts_, ty = py / ts_;
        return isSolidTile(tx, ty);
    }

    int tileSize() const { return ts_; }
    int width()    const { return w_; }
    int height()   const { return h_; }

private:
    const uint8_t* data_;
    int w_, h_, ts_;
};


#endif