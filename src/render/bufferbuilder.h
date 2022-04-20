#pragma once

#include <vector>

namespace glfr {
    class buffer_builder final {
    private:
        void *_data = nullptr;
        size_t _size = 0;
        size_t _capacity = 0;

        void ensure_capacity(size_t len) {
            if (len + _size > _capacity) {
                auto new_capacity = _capacity + (_capacity >> 1);
                while (len + _size > _capacity) {
                    new_capacity = _capacity + (_capacity >> 1);
                }

                auto *new_data = malloc(new_capacity);
                if (_data) {
                    memcpy(new_data, _data, _size);
                    free(_data);
                }
                _data = new_data;
                _capacity = new_capacity;
            }
        }

    public:
        // @formatter:off
        buffer_builder() = default;
        buffer_builder(const buffer_builder &) = delete;
        buffer_builder(buffer_builder &&) = delete;
        buffer_builder &operator=(const buffer_builder &) = delete;
        buffer_builder &operator=(buffer_builder &&) = delete;
        // @formatter:on

        ~buffer_builder() {
            if (_data) {
                free(_data);
                _capacity = 0;
                _size = 0;
                _data = nullptr;
            }
        }

        template<typename T> requires(std::is_pod_v<T>)
        buffer_builder & push(T element) {
            ensure_capacity(sizeof(T));
            *((T *) _data) = element;
            _size += sizeof(T);
            return *this;
        }
    };
}
