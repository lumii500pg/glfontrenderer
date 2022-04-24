#pragma once

#include <SDL2/SDL.h>
#include <cstdint>
#include <fmt/format.h>
#include <stdexcept>

namespace glfr {
    class bitmap final {
    private:
        SDL_Surface* _surface;
        uint32_t _width;
        uint32_t _height;

    public:
        bitmap(uint32_t width, uint32_t height)
                : _width(width), _height(height) {
            _surface = SDL_CreateRGBSurface(0, static_cast<int32_t>(width), static_cast<int32_t>(height), 32, 0xFF000000, 0xFF0000, 0xFF00, 0xFF);
            if (!_surface) {
                throw std::runtime_error(fmt::format("SDL_CreateRGBSurface failed: {}", SDL_GetError()));
            }
        }

        ~bitmap() {
            SDL_FreeSurface(_surface);
        }

        void save_to_file(const std::string_view& path) {
            SDL_SaveBMP(_surface, path.data());
        }

        [[nodiscard]] inline uint32_t& get(uint32_t x, uint32_t y) noexcept {
            auto* buffer = (uint32_t*) _surface->pixels;
            return buffer[(_height - y - 1) * _width + x];
        }

        [[nodiscard]] inline uint32_t get_width() const noexcept {
            return _width;
        }

        [[nodiscard]] inline uint32_t get_height() const noexcept {
            return _height;
        }

        [[nodiscard]] inline const void* get_data() const noexcept {
            return _surface->pixels;
        }
    };
}
