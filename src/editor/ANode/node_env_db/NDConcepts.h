#pragma once

namespace NDConcepts {

    template<typename T>
    concept DatabaseProvider = requires(const T t, typename T::StorageKey key) {
        { t.getDatabase(key) } -> std::same_as<QSqlDatabase>;
    };

    template<typename T>
    concept ByteConvertible = requires(const T t, QByteArray byteArray) {
        { T::byteArrayToClass(byteArray) } -> std::same_as<std::optional<T>>;
        { t.classToByteArray() } -> std::same_as<QByteArray>;
    };
}