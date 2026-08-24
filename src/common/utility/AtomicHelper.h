#pragma once

#include <atomic>
#include <concepts>

template<typename T>
struct AtomicHelper {
    static T get(const std::atomic<T>& ref) {
        return ref.load(std::memory_order_relaxed);
    }

    template<typename Func>
        requires std::invocable<Func, T&>
    static void set(std::atomic<T>& ref, Func&& modifier) {
        T oldData = ref.load(std::memory_order_relaxed);
        T newData;

        do {
            newData = oldData;
            modifier(newData);
        } while (!ref.compare_exchange_weak(
            oldData,
            newData,
            std::memory_order_release,
            std::memory_order_relaxed
        ));
    }
};