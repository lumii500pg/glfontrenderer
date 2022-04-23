#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <cstring>

namespace glfr {
    class font;

    class [[maybe_unused]] ttfloader {
    public:
        // @formatter:off
        static font *load_font(const char *name, size_t font_height);
        static font *load_font(const void *ptr, size_t byteLength, size_t font_height);
        // @formatter:on
    };
}