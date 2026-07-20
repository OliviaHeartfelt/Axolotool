#pragma once

#include "../../Utility/Utility.h"
#include "NDConcepts.h"
#include "NDPool.h"

namespace NDHelpers {

    template<typename ReturnType>
    struct Transaction;

    template<>
    struct Transaction<bool> {

        template<typename Func>
        static bool use(QSqlDatabase& db, Func&& fn) {
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
        static ReturnType use(QSqlDatabase& db, Func&& fn) {
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
    inline auto useTransaction(NDPool::DatabasePool& pool, Func&& fn) {
        auto lease = pool.acquire();

        using ReturnType = std::invoke_result_t<Func, QSqlQuery&>;
        return Transaction<ReturnType>::use(lease.db(), std::forward<Func>(fn));
    }

    template<typename Func>
    inline auto useQuery(NDPool::DatabasePool& pool, Func&& fn) {
        auto lease = pool.acquire();

        QSqlQuery query(lease.db());
        return std::invoke(std::forward<Func>(fn), query);
    }


    template<typename T>
    struct NullableField {
        enum class State { Null, Corrupted, Valid };

        State state = State::Null;
        std::optional<T> value = std::nullopt;

        bool isNull() const { return state == State::Null; }
        bool isCorrupted() const { return state == State::Corrupted; }
        bool isValid() const { return state == State::Valid; }
    };

    inline NullableField<muuid::uuid> parseNullableUUID(const QVariant& variant) {
        if (variant.isNull())
            return { NullableField<muuid::uuid>::State::Null, std::nullopt };

        if (const auto parsed = Utility::UUID::bytesToUuid(variant.toByteArray()))
            return { NullableField<muuid::uuid>::State::Valid, *parsed };

        return { NullableField<muuid::uuid>::State::Corrupted, std::nullopt };
    }
    template<NDConcepts::ByteConvertible T>
    inline NullableField<T> parseNullableByteConvertible(const QVariant& variant) {
        if (variant.isNull())
            return { NullableField<T>::State::Null, std::nullopt };

        const QByteArray raw = variant.toByteArray();
        const std::span<const uint8_t> bytes(reinterpret_cast<const uint8_t*>(raw.constData()), static_cast<std::size_t>(raw.size()));

        if (const auto parsed = T::bytesToClass(bytes))
            return { NullableField<T>::State::Valid, *parsed };

        return { NullableField<T>::State::Corrupted, std::nullopt };
    }
}