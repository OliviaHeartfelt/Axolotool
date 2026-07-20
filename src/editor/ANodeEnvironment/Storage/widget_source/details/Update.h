#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "Config.h"

namespace NDWidgetSourceDetails::Update {

    inline bool updateWidgetSource(QSqlQuery& query, muuid::uuid id, const NDWidgetSourceDetails::Config::UpdateWidgetSourceRecord& newProperties) {
        QStringList clauses;

        if (newProperties.id)             clauses.append("id = :new_id");
        if (newProperties.globalSourceId) clauses.append("global_source_id = :new_global_source_id");

        if (newProperties.name) clauses.append("name = :name");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE widget_source SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.id)             query.bindValue(":new_id",               Utility::UUID::uuidToBytes(*newProperties.id));
        if (newProperties.globalSourceId) query.bindValue(":new_global_source_id", Utility::UUID::uuidToBytes(*newProperties.globalSourceId));

        if (newProperties.name) query.bindValue(":name", *newProperties.name);

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update widget source:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool updateWidgetContributor(QSqlQuery& query, muuid::uuid id, const NDWidgetSourceDetails::Config::UpdateWidgetContributorRecord& newProperties) {
        QStringList clauses;

        if (newProperties.id)       clauses.append("id = :new_id");
        if (newProperties.sourceId) clauses.append("source_id = :new_source_id");

        if (newProperties.name) clauses.append("name = :name");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE widget_contributor SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.id)       query.bindValue(":new_id",        Utility::UUID::uuidToBytes(*newProperties.id));
        if (newProperties.sourceId) query.bindValue(":new_source_id", Utility::UUID::uuidToBytes(*newProperties.sourceId));

        if (newProperties.name) query.bindValue(":name", *newProperties.name);

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update widget contributor:" << query.lastError().text();
            return false;
        }
        return true;
    }
    template<NDConcepts::ByteConvertible Metadata>
    inline bool updateWidgetType(QSqlQuery& query, muuid::uuid id, const NDWidgetSourceDetails::Config::UpdateWidgetTypeRecord<Metadata>& newProperties) {
        QStringList clauses;

        if (newProperties.id)            clauses.append("id = :new_id");
        if (newProperties.contributorId) clauses.append("contributor_id = :new_contributor_id");

        if (newProperties.name) clauses.append("name = :name");

        const auto* optPtr = std::get_if<std::optional<Metadata>>(&newProperties.metadata);
        if (optPtr) clauses.append("metadata = :metadata");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE widget_type SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.id)            query.bindValue(":new_id",             Utility::UUID::uuidToBytes(*newProperties.id));
        if (newProperties.contributorId) query.bindValue(":new_contributor_id", Utility::UUID::uuidToBytes(*newProperties.contributorId));

        if (newProperties.name) query.bindValue(":name", *newProperties.name);

        if (optPtr) query.bindValue(":metadata", optPtr->has_value() ? Utility::UUID::uuidToBytes(optPtr->value().classToBytes()) : QVariant(QMetaType::fromType<QByteArray>()));

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update widget type:" << query.lastError().text();
            return false;
        }
        return true;
    }
    template<NDConcepts::ByteConvertible Data>
    inline bool updateWidgetData(QSqlQuery& query, const muuid::uuid id, const NDWidgetSourceDetails::Config::UpdateWidgetDataRecord<Data>& newProperties) {
        QStringList clauses;

        if (newProperties.id)            clauses.append("id = :new_id");
        if (newProperties.contributorId) clauses.append("contributor_id = :new_contributor_id");

        if (newProperties.name) clauses.append("name = :name");

        const auto* optPtr = std::get_if<std::optional<Data>>(&newProperties.data);
        if (optPtr) clauses.append("data = :data");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE widget_data SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.id)            query.bindValue(":new_id",             Utility::UUID::uuidToBytes(*newProperties.id));
        if (newProperties.contributorId) query.bindValue(":new_contributor_id", Utility::UUID::uuidToBytes(*newProperties.contributorId));

        if (newProperties.name) query.bindValue(":name", *newProperties.name);

        if (optPtr) query.bindValue(":data", optPtr->has_value() ? Utility::UUID::uuidToBytes(optPtr->value().classToBytes()) : QVariant(QMetaType::fromType<QByteArray>()));

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update widget data:" << query.lastError().text();
            return false;
        }
        return true;
    }
}