#include "font.h"

namespace glfr {
    font::font(const bitmap& my_bitmap, std::unordered_map<uint64_t, glyph_info> glyph_map) : _texture(my_bitmap), _glyph_map(std::move(glyph_map)) {}
}