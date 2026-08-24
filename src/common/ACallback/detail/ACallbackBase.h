#pragma once

#include <functional>
#include <utility>
#include <concepts>
#include <type_traits>

#include <QEvent>

namespace ACallbackBase {
    template<typename T>
    concept IsQEventPtr = std::derived_from<std::remove_pointer_t<T>, QEvent>&& std::is_pointer_v<T>;

    template<class... Args>
    class Callback {
        std::move_only_function<void(Args...)> callback{};

    public:
        bool hasFunction() const noexcept {
            return static_cast<bool>(this->callback);
        };

        bool set(std::move_only_function<void(Args...)> newCallback) noexcept {
            if (this->hasFunction()) 
                return false;
            this->callback = std::move(newCallback);
            return true;
        }

        void clear() noexcept {
            this->callback = {};
        };

        bool operator() (Args&... args) noexcept {
            if (not this->hasFunction())
                return false;
            try {
                this->callback(std::forward<Args>(args)...);
                return true;
            }
            catch (...) {
                return false;
            }
        }
    };

    template<IsQEventPtr T>
    class EventCallback : public Callback<T> {
    public:
        using event_type = T;
        using Callback<T>::operator();
    };

    template<IsQEventPtr T, class... Args>
    class EventArgsCallback : public Callback<T, Args> {
    public:
        using event_type = T;
        using Callback<T, Args...>::operator();
    };
}