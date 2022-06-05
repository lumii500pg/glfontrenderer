#include "gl_texture_2d.h"
#include "../bitmap.h"

namespace glfr {
    gl_texture_2d::gl_texture_2d(const bitmap& my_bitmap) : _width(my_bitmap.get_width()), _height(my_bitmap.get_height()) {
        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_2D, _id);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0,  GL_R8, my_bitmap.get_width(), my_bitmap.get_height(), 0, GL_RED, GL_UNSIGNED_BYTE, my_bitmap.get_data());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    gl_texture_2d::~gl_texture_2d() {
        glDeleteTextures(1, &_id);
    }

    void gl_texture_2d::bind(uint32_t location) noexcept {
        glActiveTexture(GL_TEXTURE0 + location);
        glBindTexture(GL_TEXTURE_2D, _id);
    }

    void gl_texture_2d::unbind(uint32_t location) noexcept {
        glActiveTexture(GL_TEXTURE0 + location);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
