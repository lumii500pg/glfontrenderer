#pragma once

#include "gl_object.h"
#include <cstdint>

namespace glfr {
    class bitmap;
    class gl_texture_2d final : public gl_object {
    private:
        uint32_t _width;
        uint32_t _height;
    public:
        [[maybe_unused]] gl_texture_2d(const bitmap& my_bitmap);
        ~gl_texture_2d();

        void bind(uint32_t location) noexcept;
        void unbind(uint32_t location) noexcept;

        [[nodiscard]] inline uint32_t get_width() const noexcept {
            return _width;
        }

        [[nodiscard]] inline uint32_t get_height() const noexcept {
            return _height;
        }
    };
}