#pragma once

#include "bitmap.h"
#include <ft2build.h>
#include FT_FREETYPE_H

class [[maybe_unused]] glfr::ttfloader {
public:
    static bitmap load_to_bitmap(const char *name, size_t font_height);
    static bitmap load_to_bitmap(const void *ptr, size_t byteLength, size_t font_height);
};
