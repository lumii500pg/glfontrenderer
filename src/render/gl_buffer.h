#pragma once

#include "gl_object.h"
#include <cstdint>

namespace glfr {
    struct vertex {
        float x, y, z, u, v;
        uint32_t color;
    };

    class gl_buffer final : public gl_object {
    private:
        GLuint _vertex_buffer;
        GLuint _index_buffer;
        size_t _index_count;
    public:
        gl_buffer(const vertex* vertices, size_t vertex_count, const uint32_t* indices, size_t index_count);
        ~gl_buffer();

        void draw() noexcept;
    };
}