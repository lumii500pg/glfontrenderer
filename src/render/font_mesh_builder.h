#pragma once

#include "gl_buffer.h"
#include <vector>
#include <string_view>

namespace glfr {
    class font;
    class gl_shader;
    class font_mesh_builder {
    private:
        std::vector<vertex> _vertices;
        std::vector<uint32_t> _indices;
        font& _my_font;

        void add_quad(const vertex &tl, const vertex &br, const vertex &tr, const vertex &bl);

    public:
        font_mesh_builder(font& my_font) : _my_font(my_font) {}
        void begin();
        void push(float x, float y, const std::string_view &message, uint32_t color);

        void end(gl_shader& shader);
    };
}
