#pragma once

#include "../../Utility/Utility.h"

namespace NDHelpers {

    template<typename ReturnType>
    struct Transaction;

    template<>
    struct Transaction<bool> {

        template<typename Func>
        static bool use(const QSqlDatabase& db, Func&& fn) {
            QSqlQuery query(db);
            Utility::SqlTransaction tr(db);

            if (!tr.started()) return false;

            if (!std::invoke(std::forward<Func>(fn), query)) return false;

            return tr.commit();
        }
    };

    template<typename T>
    struct Transaction<std::optional<T>> {

        using ReturnType = std::optional<T>;

        template<typename Func>
        static ReturnType use(const QSqlDatabase& db, Func&& fn) {
            QSqlQuery query(db);
            Utility::SqlTransaction tr(db);

            if (!tr.started()) return ReturnType(std::nullopt);

            auto result = std::invoke(std::forward<Func>(fn), query);
            if (!result) return ReturnType(std::nullopt);

            if (!tr.commit()) return ReturnType(std::nullopt);
            return result;
        }
    };

    template<typename Func>
    auto useTransaction(const QSqlDatabase& db, Func&& fn) {
        using ReturnType = std::invoke_result_t<Func, QSqlQuery&>;
        return Transaction<ReturnType>::use(db, std::forward<Func>(fn));
    }

    template<typename Func>
    auto useQuery(const QSqlDatabase& db, Func&& fn) {
        QSqlQuery query(db);
        return std::invoke(std::forward<Func>(fn), query);
    }
}