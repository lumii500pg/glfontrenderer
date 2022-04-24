#pragma once

#include "render/gl_texture_2d.h"
#include <cstdint>
#include <unordered_map>

namespace glfr {
    struct glyph_info final {
        uint64_t value;
        float x_advance, y_advance;
        float width, height;
        float offset_x, offset_y;
        float tex_coord_x, tex_coord_y;

        glyph_info() = default;

        glyph_info(uint64_t value, float x_advance, float y_advance, float width, float height,
                   float offset_x, float offset_y, float tex_coord_x, float tex_coord_y) :
                value(value), x_advance(x_advance), y_advance(y_advance), width(width), height(height),
                offset_x(offset_x), offset_y(offset_y), tex_coord_x(tex_coord_x), tex_coord_y(tex_coord_y) {}
    };

    class font final {
        gl_texture_2d _texture;
        std::unordered_map<uint64_t, glyph_info> _glyph_map;
        float _font_height;
    public:
        font(const bitmap &my_bitmap, std::unordered_map<uint64_t, glyph_info> glyph_map, float font_height);

        [[nodiscard]] inline const glyph_info *get_glyph(uint64_t code) const {
            const auto iterator = _glyph_map.find(code);
            return iterator == _glyph_map.end() ? nullptr : &iterator->second;
        }

        [[nodiscard]] inline gl_texture_2d &get_texture() noexcept {
            return _texture;
        }

        [[nodiscard]] inline const gl_texture_2d &get_texture() const noexcept {
            return _texture;
        }

        [[nodiscard]] inline float get_height() const noexcept {
            return _font_height;
        }
    };
}
