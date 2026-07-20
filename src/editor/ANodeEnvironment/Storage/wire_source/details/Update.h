#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "Config.h"

namespace NDWireSourceDetails::Update {

    inline bool updateWireSource(QSqlQuery& query, muuid::uuid id, const NDWireSourceDetails::Config::UpdateWireSourceRecord& newProperties) {
        QStringList clauses;

        if (newProperties.id)             clauses.append("id = :new_id");
        if (newProperties.globalSourceId) clauses.append("global_source_id = :new_global_source_id");

        if (newProperties.name) clauses.append("name = :name");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE wire_source SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.id)             query.bindValue(":new_id",               Utility::UUID::uuidToBytes(*newProperties.id));
        if (newProperties.globalSourceId) query.bindValue(":new_global_source_id", Utility::UUID::uuidToBytes(*newProperties.globalSourceId));

        if (newProperties.name)  query.bindValue(":name", newProperties.name ? *newProperties.name : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update wire source:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool updateWireContributor(QSqlQuery& query, muuid::uuid id, const NDWireSourceDetails::Config::UpdateWireContributorRecord& newProperties) {
        QStringList clauses;

        if (newProperties.id)       clauses.append("id = :new_id");
        if (newProperties.sourceId) clauses.append("source_id = :new_source_id");

        if (newProperties.name) clauses.append("name = :name");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE wire_contributor SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.id)       query.bindValue(":new_id", Utility::UUID::uuidToBytes(*newProperties.id));
        if (newProperties.sourceId) query.bindValue(":new_source_id", Utility::UUID::uuidToBytes(*newProperties.sourceId));

        if (newProperties.name)  query.bindValue(":name", newProperties.name ? *newProperties.name : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update wire contributor:" << query.lastError().text();
            return false;
        }
        return true;
    }
    template<NDConcepts::ByteConvertible Metadata>
    inline bool updateWireStyle(QSqlQuery& query, muuid::uuid id, const NDWireSourceDetails::Config::UpdateWireStyleRecord<Metadata>& newProperties) {
        QStringList clauses;

        if (newProperties.id)            clauses.append("id = :new_id");
        if (newProperties.contributorId) clauses.append("contributor_id = :new_contributor_id");

        if (newProperties.name)          clauses.append("name = :name");
        if (newProperties.color)         clauses.append("color = :color");
        if (newProperties.wireThickness) clauses.append("wire_thickness = :wire_thickness");

        const auto* optPtr = std::get_if<std::optional<Metadata>>(&newProperties.metadata);
        if (optPtr) clauses.append("metadata = :metadata");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE wire_style SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.id)            query.bindValue(":new_id", Utility::UUID::uuidToBytes(*newProperties.id));
        if (newProperties.contributorId) query.bindValue(":new_contributor_id", Utility::UUID::uuidToBytes(*newProperties.contributorId));

        if (newProperties.name)          query.bindValue(":name",           *newProperties.name);
        if (newProperties.color)         query.bindValue(":color",          static_cast<int>(newProperties.color->rgba()));
        if (newProperties.wireThickness) query.bindValue(":wire_thickness", *newProperties.wireThickness);

        if (optPtr) query.bindValue(":metadata", optPtr->has_value() ? Utility::UUID::uuidToBytes(optPtr->value().classToBytes()) : QVariant(QMetaType::fromType<QByteArray>()));

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update wire style:" << query.lastError().text();
            return false;
        }
        return true;
    }
    template<NDConcepts::ByteConvertible Data>
    inline bool updateWireData(QSqlQuery& query, muuid::uuid id, const NDWireSourceDetails::Config::UpdateWireDataRecord<Data>& newProperties) {
        QStringList clauses;

        if (newProperties.id)            clauses.append("id = :new_id");
        if (newProperties.contributorId) clauses.append("contributor_id = :new_contributor_id");

        if (newProperties.name) clauses.append("name = :name");

        const auto* optPtr = std::get_if<std::optional<Data>>(&newProperties.data);
        if (optPtr) clauses.append("data = :data");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE wire_data SET %1 WHERE id = :id;").arg(clauses.join(", "));
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
            qCritical() << "Failed to execute dynamic update wire data:" << query.lastError().text();
            return false;
        }
        return true;
    }
}