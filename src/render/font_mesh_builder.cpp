#include "font_mesh_builder.h"
#include "../text/utf8_iterator.h"
#include "../font.h"
#include "gl_shader.h"

namespace glfr {
    void font_mesh_builder::add_quad(const vertex &tl, const vertex &br, const vertex &tr, const vertex &bl) {
        const auto index = _vertices.size();

        _vertices.push_back(tl);
        _vertices.push_back(bl);
        _vertices.push_back(br);
        _vertices.push_back(tr);

        _indices.push_back(index);
        _indices.push_back(index + 1);
        _indices.push_back(index + 3);
        _indices.push_back(index + 3);
        _indices.push_back(index + 1);
        _indices.push_back(index + 2);
    }

    void font_mesh_builder::begin() {
        _vertices.clear();
        _indices.clear();
    }

    void font_mesh_builder::push(float x, float y, const std::string_view &message, uint32_t color) {
        const float font_height = _my_font.get_height();
        const float font_tex_w = static_cast<float>(_my_font.get_texture().get_width());
        const float font_tex_h = static_cast<float>(_my_font.get_texture().get_height());

        float offset_x = 0;
        float offset_y = 0;

        bool first_glyph_of_line = true;

        Utf8Iterator iterator(message.data());
        int i = 0;
        while (iterator.has_chars()) {
            const auto current = *iterator;
            ++iterator;

            if (current == '\r') continue;
            if (current == '\n') {
                offset_y += font_height; //TOOD: CIV
                offset_x = 0.0f;
                first_glyph_of_line = true;
                continue;
            }

            const auto *glyph = _my_font.get_glyph(current);
            if (glyph == nullptr) continue;

            float x_char_offset = glyph->offset_x;
            if (first_glyph_of_line) {
                if (glyph->offset_x < 0.0f) {
                    x_char_offset = 0.0f;
                }
                first_glyph_of_line = false;
            }

            vertex vertex_tl;
            vertex_tl.x = x + offset_x + x_char_offset;
            vertex_tl.y = y + offset_y + font_height - glyph->offset_y;
            vertex_tl.u = glyph->tex_coord_x / font_tex_w;
            vertex_tl.v = glyph->tex_coord_y / font_tex_h;
            vertex_tl.color = color;

            vertex vertex_bl;
            vertex_bl.x = vertex_tl.x;
            vertex_bl.y = vertex_tl.y + glyph->height;
            vertex_bl.u = glyph->tex_coord_x / font_tex_w;
            vertex_bl.v = (glyph->tex_coord_y + glyph->height) / font_tex_h;
            vertex_bl.color = color;

            vertex vertex_br;
            vertex_br.x = vertex_tl.x + glyph->width;
            vertex_br.y = vertex_tl.y + glyph->height;
            vertex_br.u = (glyph->tex_coord_x + glyph->width) / font_tex_w;
            vertex_br.v = (glyph->tex_coord_y + glyph->height) / font_tex_h;
            vertex_br.color = color;

            vertex vertex_tr;
            vertex_tr.x = vertex_tl.x + glyph->width;
            vertex_tr.y = vertex_tl.y;
            vertex_tr.u = (glyph->tex_coord_x + glyph->width) / font_tex_w;
            vertex_tr.v = glyph->tex_coord_y / font_tex_h;
            vertex_tr.color = color;

            add_quad(vertex_tl, vertex_br, vertex_tr, vertex_bl);

            offset_x += glyph->x_advance;
            offset_y += glyph->y_advance;
        }
    }

    void font_mesh_builder::end(gl_shader& shader) {
        gl_buffer buffer(_vertices.data(), _vertices.size(), _indices.data(), _indices.size());

        shader.bind();
        shader.uniform_2f("resolution", 1600, 900); //TODO:

        _my_font.get_texture().bind(0);

        buffer.draw();

        _my_font.get_texture().unbind(0);
        shader.unbind();
    }
}
