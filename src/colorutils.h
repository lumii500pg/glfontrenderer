#pragma once

#include <cstdlib>
#include <array>

namespace glfr {
    static const std::array<float, 4> rgba(u_int32_t color) {
        return {
                static_cast<float>(((color >> 16) & 0xff) / 255.),
                static_cast<float>(((color >> 8) & 0xff) / 255.),
                static_cast<float>(((color) & 0xff) / 255.),
                static_cast<float>(((color >> 24) & 0xff) / 255.)
        };
    }

    static const u_int32_t dez(float r, float g, float b, float a) {
        return (((int) (a * 255) & 0xFF) << 24) |
               (((int) (r * 255) & 0xFF) << 16) |
               (((int) (g * 255) & 0xFF) << 8) |
               (((int) (b * 255) & 0xFF));
    }
}
