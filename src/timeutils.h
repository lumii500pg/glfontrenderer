#pragma once

#include <chrono>
#include <type_traits>

/**
 * @author KitsuneAlex
 */
namespace glfr {
    template<typename D>
    requires(std::chrono::_Is_duration_v<D>)
    static inline std::uint64_t current_time() noexcept {
        const auto time = std::chrono::high_resolution_clock::now().time_since_epoch();
        const auto milli_time = std::chrono::duration_cast<D>(time);
        return static_cast<std::uint64_t>(milli_time.count());
    }
}
