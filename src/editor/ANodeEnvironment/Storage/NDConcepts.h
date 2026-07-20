#pragma once

#include "NDPool.h"

namespace NDConcepts {

    template<typename T>
    concept DatabaseProvider = requires(const T t) {
        { t.getPool() } -> std::convertible_to<NDPool::DatabasePool&>;
    };

    template<typename T>
    concept ByteConvertible = requires(const T t, std::span<const uint8_t> bytes) {
        { T::bytesToClass(bytes) } -> std::same_as<std::optional<T>>;
        { t.classToBytes() } -> std::same_as<std::vector<uint8_t>>;
    };
}