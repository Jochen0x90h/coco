#pragma once

#include <utility>


namespace coco {

/// @brief Lightweight method callback.
///
template<typename... Args>
struct Callback {
    void *object;
    void (*method)(void *, Args...);

    void operator ()(Args... args) const {
        method(object, std::forward<Args>(args)...);
    }
};

/// @brief Create a callback.
/// If there is a class Foo with method bar() then call makeCallback<Foo, &Foo::bar>(&foo) where foo is an instance
/// of Foo.
template <typename T, auto M, typename... Args>
inline Callback<Args...> makeCallback(void *object) {
    return {
        object,
        [](void *object, Args... args) {
            T* self = static_cast<T *>(object);
            (self->*M)(std::forward<Args>(args)...);
        }
    };
}

} // namespace coco
