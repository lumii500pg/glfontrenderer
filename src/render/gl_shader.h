#pragma once

#include "gl_object.h"
#include <string_view>

namespace glfr {
    class gl_shader final : public gl_object {
    public:
        [[maybe_unused]] gl_shader();
        [[maybe_unused]] gl_shader(const std::string_view& vertex_source, const std::string_view& fragment_source);
        ~gl_shader();

        void bind() noexcept;
        void unbind() noexcept;
    };
}