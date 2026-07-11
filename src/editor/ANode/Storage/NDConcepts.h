#pragma once

#include "NDPool.h"

namespace NDConcepts {

    template<typename T>
    concept DatabaseProvider = requires(const T t) {
        { t.getPool() } -> std::same_as<NDPool::DatabasePool&>;
    };

    template<typename T>
    concept ByteConvertible = requires(const T t, QByteArray byteArray) {
        { T::byteArrayToClass(byteArray) } -> std::same_as<std::optional<T>>;
        { t.classToByteArray() } -> std::same_as<QByteArray>;
    };
}