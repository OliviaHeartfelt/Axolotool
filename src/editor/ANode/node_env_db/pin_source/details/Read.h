#pragma once

#include "../../../../Utility/Utility.h"
#include "Config.h"

namespace NDPinSourceDetails::Read {

    // 0. Source
    inline std::optional<NDPinSourceDetails::Config::Source> getSource(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT name
            FROM pin_source 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getSource query:" << query.lastError().text();
            return std::nullopt;
        }

        if (!query.next()) return std::nullopt;

        return NDPinSourceDetails::Config::Source{
            id,
            query.value(0).toString()
        };
    }

    // 1. Flow
    inline std::optional<NDPinSourceDetails::Config::Flow> getFlow(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT source_id, name, degree
            FROM flow 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getFlow query:" << query.lastError().text();
            return std::nullopt;
        }

        if (!query.next()) return std::nullopt;

        const auto sourceId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!sourceId) return std::nullopt;

        return NDPinSourceDetails::Config::Flow{
            id,
            *sourceId,
            query.value(1).toString(),
            query.value(2).toReal()
        };
    }
    inline std::optional<QList<NDPinSourceDetails::Config::Flow>> getAllFlows(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDPinSourceDetails::Config::Flow> list;

        query.prepare(R"(
            SELECT id, name, degree
            FROM flow 
            WHERE source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllFlows query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            if (!id) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDPinSourceDetails::Config::Flow{
                *id,
                sourceId,
                query.value(1).toString(),
                query.value(2).toReal()
            });
        }
        return list;
    }

    // 2. Type
    inline std::optional<NDPinSourceDetails::Config::Type> getType(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT source_id, name, bit_size
            FROM type 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getType query:" << query.lastError().text();
            return std::nullopt;
        }

        if (!query.next()) return std::nullopt;

        const auto sourceId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!sourceId) return std::nullopt;

        return NDPinSourceDetails::Config::Type{
            id,
            *sourceId,
            query.value(1).toString(),
            query.value(2).toInt()
        };
    }
    inline std::optional<QList<NDPinSourceDetails::Config::Type>> getAllTypes(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDPinSourceDetails::Config::Type> list;

        query.prepare(R"(
            SELECT id, name, bit_size
            FROM type 
            WHERE source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllTypes query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            if (!id) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDPinSourceDetails::Config::Type{
                *id,
                sourceId,
                query.value(1).toString(),
                query.value(2).toInt()
            });
        }
        return list;
    }

    // 3. Style
    inline std::optional<NDPinSourceDetails::Config::Style> getStyle(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT source_id, name, color, wire_thickness
            FROM style 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getStyle query:" << query.lastError().text();
            return std::nullopt;
        }

        if (!query.next()) return std::nullopt;

        const auto sourceId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!sourceId) return std::nullopt;

        return NDPinSourceDetails::Config::Style{
            id,
            *sourceId,
            query.value(1).toString(),
            QColor::fromRgba(query.value(2).toUInt()),
            query.value(3).toInt()
        };
    }
    inline std::optional<QList<NDPinSourceDetails::Config::Style>> getAllStyles(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDPinSourceDetails::Config::Style> list;

        query.prepare(R"(
            SELECT id, name, color, wire_thickness
            FROM style 
            WHERE source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllStyles query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());

            if (!id) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDPinSourceDetails::Config::Style{
                *id,
                sourceId,
                query.value(1).toString(),
                QColor::fromRgba(query.value(2).toUInt()),
                query.value(3).toInt()
            });
        }
        return list;
    }
}