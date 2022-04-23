#pragma once

#include <GL/glew.h>

namespace glfr {
    class gl_object {
    protected:
        GLuint _id;

    public:
        [[nodiscard]] inline GLuint get_id() const noexcept {
            return _id;
        }
    };
}
