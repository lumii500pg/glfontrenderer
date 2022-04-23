#include "gl_shader.h"
#include "../logger.h"

#include <stdexcept>

static const char *_VERTEX_SOURCE = R"(#version 330

    #ifdef GL_ES
    precision highp float;
    #endif

    in vec3 position;
    in vec2 texCoord;
    in uint color;

    out vec2 texCoordFS;
    out vec4 colorFS;

    void main() {
        gl_Position = vec4(position, 1.);
        texCoordFS = texCoord;
        colorFS = vec4(1.0);
        //colorFS = (vec4(float(((color >> 16) & 0xff)), float((color >> 8) & 0xff), float((color & 0xff)), float(((color >> 24) & 0xff))) / vec4(255.));
    })";

static const char *_FRAGMENT_SOURCE = R"(#version 330

    #ifdef GL_ES
    precision highp float;
    #endif

    in vec2 texCoordFS;
    in vec4 colorFS;

    uniform sampler2D fontTexture;

    out vec4 outColor;

    void main(void) {
        outColor = colorFS * texture(fontTexture, texCoordFS);
    })";

namespace glfr {
    inline GLuint create_shader(GLenum shader_type, const std::string_view &source) {
        auto shader = glCreateShader(shader_type);
        if (!shader) {
            throw std::runtime_error("glCreateShader failed");
        }
        const auto source_ptr = source.data();
        glShaderSource(shader, 1, &source_ptr, nullptr);
        glCompileShader(shader);
        GLint compile_status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
        if (compile_status != GL_TRUE) {
            GLsizei info_log_length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);

            std::string info_log;
            info_log.resize(info_log_length);
            glGetShaderInfoLog(shader, info_log_length, &info_log_length, info_log.data());

            logger::error("couldn't compile shader: {}", info_log);
            throw std::runtime_error("glCompileShader failed");
        }
        return shader;
    }

    [[maybe_unused]] gl_shader::gl_shader() : gl_shader(_VERTEX_SOURCE, _FRAGMENT_SOURCE) {}

    [[maybe_unused]] gl_shader::gl_shader(const std::string_view &vertex_source,
                                          const std::string_view &fragment_source) {
        _id = glCreateProgram();

        logger::info("creating vertex...");
        auto vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_source);
        glAttachShader(_id, vertex_shader); // vertex
        logger::info("creating fragment...");
        auto fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_source);
        glAttachShader(_id, fragment_shader); // fragment

        glLinkProgram(_id);
        GLint link_status;
        glGetProgramiv(_id, GL_LINK_STATUS, &link_status);
        if (link_status != GL_TRUE) {
            GLsizei info_log_length;
            glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &info_log_length);

            std::string info_log;
            info_log.resize(info_log_length);
            glGetProgramInfoLog(_id, info_log_length, &info_log_length, info_log.data());
            logger::error("couldn't link shader! {}", info_log);
        }

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }

    gl_shader::~gl_shader() { glDeleteProgram(_id); }

    [[maybe_unused]] void gl_shader::bind() noexcept {
        glUseProgram(_id);
    }

    [[maybe_unused]] void gl_shader::unbind() noexcept {
        glUseProgram(0);
    }
}
