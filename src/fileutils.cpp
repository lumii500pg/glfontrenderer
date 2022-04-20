#include "fileutils.h"

#include <stdexcept>

namespace glfr::file_utils {
    std::vector<int8_t> read_bytes(const std::string_view &path) {
        auto *file = fopen(path.data(), "rb");
        if (!file) {
            throw std::runtime_error("fopen");
        }

        fseek(file, 0, SEEK_END);
        const auto length = ftell(file);
        if (!length) {
            throw std::runtime_error("ftell");
        }
        fseek(file, 0, SEEK_SET);

        std::vector<int8_t> buffer(length);
        const auto length_read = fread(buffer.data(), 1, length, file);
        if (length != length_read) {
            throw std::runtime_error("fread");
        }

        fclose(file);
        return buffer;
    }
}
