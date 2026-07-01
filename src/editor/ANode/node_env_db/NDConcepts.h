#pragma once

namespace NDConcepts {

    template<typename T>
    concept DatabaseProvider = requires(const T t, typename T::StorageKey key) {
        { t.getDatabase(key) } -> std::same_as<QSqlDatabase>;
    };
}