#pragma once

#include "bitmap.h"

#include <cstdint>
#include <unordered_map>
#include <GL/glew.h>

namespace glfr {
    struct glyph_info final {
        uint64_t value;
        float x_advance;
        float y_advance;
        float width;
        float height;
        float offset_x;
        float offset_y;
        float tex_coord_x;
        float tex_coord_y;

        glyph_info() = default;

        glyph_info(uint64_t value, float x_advance, float y_advance, float width, float height,
                   float offset_x, float offset_y, float tex_coord_x, float tex_coord_y) :
                value(value), x_advance(x_advance), y_advance(y_advance), width(width), height(height),
                offset_x(offset_x), offset_y(offset_y), tex_coord_x(tex_coord_x), tex_coord_y(tex_coord_y) {}
    };

    class font final {
        GLuint _glTextureId;
        const char *_name;
        bitmap _my_bitmap;

        std::unordered_map<uint64_t, glyph_info> _glyph_map;

        font(const char *name, bitmap my_bitmap, std::unordered_map<uint64_t, glyph_info> glyph_map)
                : _name(name), _my_bitmap(my_bitmap) {}

        void draw(const char *input, const uint_fast16_t color) {
        }
    };
}
