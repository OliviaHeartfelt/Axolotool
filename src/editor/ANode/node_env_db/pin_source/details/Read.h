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
    inline std::optional<QList<NDPinSourceDetails::Config::Source>> getAllSources(QSqlQuery& query, const bool continueAtFail = false) {
        QList<NDPinSourceDetails::Config::Source> list;

        query.prepare(R"(
            SELECT id, name
            FROM flow
        )");

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllContributors query:" << query.lastError().text();
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

            list.append(NDPinSourceDetails::Config::Source{
                *id,
                query.value(1).toString()
            });
        }
        return list;
    }

    inline std::optional<NDPinSourceDetails::Config::Contributor> getContributor(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT source_id, name
            FROM pin_contributor 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getContributor query:" << query.lastError().text();
            return std::nullopt;
        }

        if (!query.next()) return std::nullopt;

        const auto sourceId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!sourceId) return std::nullopt;

        return NDPinSourceDetails::Config::Contributor{
            id,
            *sourceId,
            query.value(1).toString()
        };
    }
    inline std::optional<QList<NDPinSourceDetails::Config::Contributor>> getAllContributors(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDPinSourceDetails::Config::Contributor> list;

        query.prepare(R"(
            SELECT id, name
            FROM flow 
            WHERE source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllContributors query:" << query.lastError().text();
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

            list.append(NDPinSourceDetails::Config::Contributor{
                *id,
                sourceId,
                query.value(1).toString()
            });
        }
        return list;
    }

    // 1. Flow
    inline std::optional<NDPinSourceDetails::Config::Flow> getFlow(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT contributor_id, name, degree
            FROM flow 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getFlow query:" << query.lastError().text();
            return std::nullopt;
        }

        if (!query.next()) return std::nullopt;

        const auto contributorId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!contributorId) return std::nullopt;

        return NDPinSourceDetails::Config::Flow{
            id,
            *contributorId,
            query.value(1).toString(),
            query.value(2).toReal()
        };
    }
    inline std::optional<QList<NDPinSourceDetails::Config::Flow>> getContributorFlows(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
        QList<NDPinSourceDetails::Config::Flow> list;

        query.prepare(R"(
            SELECT id, name, degree
            FROM flow 
            WHERE contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

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
                contributorId,
                query.value(1).toString(),
                query.value(2).toReal()
            });
        }
        return list;
    }
    inline std::optional<QList<NDPinSourceDetails::Config::Flow>> getAllFlows(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDPinSourceDetails::Config::Flow> list;

        query.prepare(R"(
            SELECT f.id, f.contributor_id, f.name, f.degree
            FROM flow f
            INNER JOIN pin_contributor c ON f.contributor_id = c.id
            WHERE c.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllFlows query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto contributorId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());

            if (!id || !contributorId) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDPinSourceDetails::Config::Flow{
                *id,
                *contributorId,
                query.value(2).toString(),
                query.value(3).toReal()
            });
        }
        return list;
    }

    // 2. Type
    inline std::optional<NDPinSourceDetails::Config::Type> getType(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT contributor_id, name, bit_size
            FROM type 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getType query:" << query.lastError().text();
            return std::nullopt;
        }

        if (!query.next()) return std::nullopt;

        const auto contributorId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!contributorId) return std::nullopt;

        return NDPinSourceDetails::Config::Type{
            id,
            *contributorId,
            query.value(1).toString(),
            query.value(2).toInt()
        };
    }
    inline std::optional<QList<NDPinSourceDetails::Config::Type>> getContributorTypes(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
        QList<NDPinSourceDetails::Config::Type> list;

        query.prepare(R"(
            SELECT id, name, bit_size
            FROM type 
            WHERE contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getContributorTypes query:" << query.lastError().text();
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
                contributorId,
                query.value(1).toString(),
                query.value(2).toInt()
            });
        }
        return list;
    }
    inline std::optional<QList<NDPinSourceDetails::Config::Type>> getAllTypes(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDPinSourceDetails::Config::Type> list;

        query.prepare(R"(
            SELECT t.id, t.contributor_id, t.name, t.bit_size
            FROM type t
            INNER JOIN pin_contributor c ON t.contributor_id = c.id
            WHERE c.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllTypes query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto contributorId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());

            if (!id || !contributorId) {
                if (continueAtFail) continue;
                else return std::nullopt;
            }

            list.append(NDPinSourceDetails::Config::Type{
                *id,
                *contributorId,
                query.value(2).toString(),
                query.value(3).toInt()
            });
        }
        return list;
    }

    // 3. Style
    inline std::optional<NDPinSourceDetails::Config::Style> getStyle(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT contributor_id, name, color, wire_thickness
            FROM style 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getStyle query:" << query.lastError().text();
            return std::nullopt;
        }

        if (!query.next()) return std::nullopt;

        const auto contributorId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!contributorId) return std::nullopt;

        return NDPinSourceDetails::Config::Style{
            id,
            *contributorId,
            query.value(1).toString(),
            QColor::fromRgba(query.value(2).toUInt()),
            query.value(3).toInt()
        };
    }
    inline std::optional<QList<NDPinSourceDetails::Config::Style>> getContributorStyles(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
        QList<NDPinSourceDetails::Config::Style> list;

        query.prepare(R"(
            SELECT id, name, color, wire_thickness
            FROM style 
            WHERE contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

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
                contributorId,
                query.value(1).toString(),
                QColor::fromRgba(query.value(2).toUInt()),
                query.value(3).toInt()
            });
        }
        return list;
    }
    inline std::optional<QList<NDPinSourceDetails::Config::Style>> getAllStyles(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDPinSourceDetails::Config::Style> list;

        query.prepare(R"(
            SELECT s.id, s.contributor_id, s.name, s.color, s.wire_thickness
            FROM style s
            INNER JOIN pin_contributor c ON s.contributor_id = c.id
            WHERE c.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllStyles query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto contributorId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());

            if (!id || !contributorId) {
                if (continueAtFail) continue;
                else return std::nullopt;
            }

            list.append(NDPinSourceDetails::Config::Style{
                *id,
                *contributorId,
                query.value(2).toString(),
                QColor::fromRgba(query.value(3).toUInt()),
                query.value(4).toInt()
            });
        }
        return list;
    }
}