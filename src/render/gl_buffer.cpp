#include "gl_buffer.h"

namespace glfr {
    gl_buffer::gl_buffer(const vertex *vertices, size_t vertex_count, const uint32_t *indices, size_t index_count) : _index_count(index_count) {
        glGenVertexArrays(1, &_id);
        glBindVertexArray(_id);

        glGenBuffers(1, &_index_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(uint32_t), indices, GL_STATIC_DRAW);

        glGenBuffers(1, &_vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(vertex), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), nullptr);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void *) (2 * sizeof(float)));
        glVertexAttribPointer(2, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(vertex), (const void *) (4 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

    gl_buffer::~gl_buffer() {
        glDeleteVertexArrays(1, &_id);
        glDeleteBuffers(1, &_index_buffer);
        glDeleteBuffers(1, &_vertex_buffer);
    }

    void gl_buffer::draw() noexcept {
        glBindVertexArray(_id);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glDrawElements(GL_TRIANGLES, _index_count, GL_UNSIGNED_INT, nullptr);

        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(1);
        glDisableVertexAttribArray(0);

        glBindVertexArray(0);
    }
}
