#pragma once

#include "gl_object.h"
#include <cstdint>

namespace glfr {
    class bitmap;
    class gl_texture_2d final : public gl_object {
    public:
        [[maybe_unused]] gl_texture_2d(const bitmap& my_bitmap);
        ~gl_texture_2d();

        void bind(uint32_t location) noexcept;
        void unbind(uint32_t location) noexcept;
    };
}