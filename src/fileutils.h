#pragma once

#include <string_view>
#include <vector>

namespace glfr::file_utils {
    std::vector<int8_t> read_bytes(const std::string_view &path);
}