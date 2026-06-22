#pragma once

namespace NDConcepts {

    template<typename T>
    concept DatabaseProvider = requires(const T a, typename T::StorageKey key, const QStringList& str) {
        { a.getDatabase(key) } -> std::same_as<QSqlDatabase>;
        { a.containsAllCoreTables(str) } -> std::same_as<bool>;
    };
}