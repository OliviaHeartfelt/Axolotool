#pragma once

#include "../../../../Utility/Utility.h"
#include "Config.h"

namespace NDWidgetSourceDetails::Update {

    inline bool updateWidgetSource(QSqlQuery& query, muuid::uuid id, const NDWidgetSourceDetails::Config::UpdateWidgetSourceRecord& newProperties) {
        QStringList clauses;

        if (newProperties.name) clauses.append("name = :name");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE widget_source SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));
        if (newProperties.name)  query.bindValue(":name", newProperties.name ? *newProperties.name : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update widget source:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool updateWidgetContributor(QSqlQuery& query, muuid::uuid id, const NDWidgetSourceDetails::Config::UpdateWidgetContributorRecord& newProperties) {
        QStringList clauses;

        if (newProperties.name) clauses.append("name = :name");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE widget_contributor SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));
        if (newProperties.name) query.bindValue(":name", newProperties.name ? *newProperties.name : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update widget contributor:" << query.lastError().text();
            return false;
        }
        return true;
    }
    template<NDWidgetSourceDetails::Config::ByteConvertible Metadata>
    inline bool updateWidgetType(QSqlQuery& query, muuid::uuid id, const NDWidgetSourceDetails::Config::UpdateWidgetTypeRecord<Metadata>& newProperties) {
        QStringList clauses;

        if (newProperties.name) clauses.append("name = :name");
        if (std::holds_alternative<std::optional<Metadata>>(newProperties.metadata)) clauses.append("metadata = :metadata");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE widget_type SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));
        if (newProperties.name) query.bindValue(":name", *newProperties.name);

        if (const auto* optPtr = std::get_if<std::optional<Metadata>>(&newProperties.metadata)) {
            if (*optPtr) {
                query.bindValue(":metadata", QVariant((*optPtr)->classToByteArray()));
            }
            else {
                query.bindValue(":metadata", QVariant(QMetaType::fromType<QByteArray>()));
            }
        }

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update widget type:" << query.lastError().text();
            return false;
        }
        return true;
    }
    template<NDWidgetSourceDetails::Config::ByteConvertible Data>
    inline bool updateWidgetData(QSqlQuery& query, muuid::uuid id, const NDWidgetSourceDetails::Config::UpdateWidgetDataRecord<Data>& newProperties) {
        QStringList clauses;

        if (newProperties.name) clauses.append("name = :name");
        if (std::holds_alternative<std::optional<Data>>(newProperties.data)) clauses.append("data = :data");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE widget_data SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));
        if (newProperties.name) query.bindValue(":name", *newProperties.name);

        if (const auto* optPtr = std::get_if<std::optional<Data>>(&newProperties.data)) {
            if (*optPtr) {
                query.bindValue(":data", (*optPtr)->classToByteArray());
            }
            else {
                query.bindValue(":data", QVariant(QMetaType::fromType<QByteArray>())); // Explicit DB NULL
            }
        }

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update widget data:" << query.lastError().text();
            return false;
        }
        return true;
    }
}

/*
CREATE TABLE IF NOT EXISTS widget_source (
    id   BLOB PRIMARY KEY,
    name TEXT NOT NULL,
    UNIQUE(name)
);
CREATE TABLE IF NOT EXISTS widget_contributor (
    id        BLOB PRIMARY KEY,
    source_id BLOB NOT NULL REFERENCES widget_source(id) ON DELETE CASCADE,
    name      TEXT NOT NULL,
    UNIQUE(source_id, name)
);
CREATE TABLE IF NOT EXISTS widget_type (
    id             BLOB PRIMARY KEY,
    contributor_id BLOB NOT NULL REFERENCES widget_source(id) ON DELETE CASCADE,
    name           TEXT NOT NULL,
    metadata       BLOB,
    UNIQUE(contributor_id, name)
);
CREATE TABLE IF NOT EXISTS widget_data (
    id             BLOB PRIMARY KEY,
    contributor_id BLOB NOT NULL REFERENCES widget_source(id) ON DELETE CASCADE,
    name           TEXT NOT NULL,
    data           BLOB,
    UNIQUE(contributor_id, name)
);
*/