#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "Config.h"

namespace NDWireSourceDetails::Update {

    inline bool updateWireSource(QSqlQuery& query, muuid::uuid id, const NDWireSourceDetails::Config::UpdateWireSourceRecord& newProperties) {
        QStringList clauses;

        if (newProperties.name) clauses.append("name = :name");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE wire_source SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.name)  query.bindValue(":name", newProperties.name ? *newProperties.name : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update wire source:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool updateWireContributor(QSqlQuery& query, muuid::uuid id, const NDWireSourceDetails::Config::UpdateWireContributorRecord& newProperties) {
        QStringList clauses;

        if (newProperties.name) clauses.append("name = :name");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE wire_contributor SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

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

        if (newProperties.name)          clauses.append("name = :name");
        if (newProperties.color)         clauses.append("color = :color");
        if (newProperties.wireThickness) clauses.append("wire_thickness = :wire_thickness");
        if (std::holds_alternative<std::optional<Metadata>>(newProperties.metadata)) clauses.append("metadata = :metadata");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE wire_style SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.name)          query.bindValue(":name",           *newProperties.name);
        if (newProperties.color)         query.bindValue(":color",          static_cast<int>(newProperties.color->rgba()));
        if (newProperties.wireThickness) query.bindValue(":wire_thickness", *newProperties.wireThickness);

        if (const auto* optPtr = std::get_if<std::optional<Metadata>>(&newProperties.metadata)) {
            if (optPtr->has_value())
                query.bindValue(":metadata", QVariant(optPtr->value().classToByteArray()));
            else
                query.bindValue(":metadata", QVariant(QMetaType::fromType<QByteArray>()));
        }

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update wire style:" << query.lastError().text();
            return false;
        }
        return true;
    }
    template<NDConcepts::ByteConvertible Data>
    inline bool updateWireData(QSqlQuery& query, muuid::uuid id, const NDWireSourceDetails::Config::UpdateWireDataRecord<Data>& newProperties) {
        QStringList clauses;

        if (newProperties.name) clauses.append("name = :name");
        if (std::holds_alternative<std::optional<Data>>(newProperties.data)) clauses.append("data = :data");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE wire_data SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.name) query.bindValue(":name", *newProperties.name);

        if (const auto* optPtr = std::get_if<std::optional<Data>>(&newProperties.data)) {
            if (optPtr->has_value())
                query.bindValue(":data", QVariant(optPtr->value().classToByteArray()));
            else
                query.bindValue(":data", QVariant(QMetaType::fromType<QByteArray>()));
        }

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update wire data:" << query.lastError().text();
            return false;
        }
        return true;
    }
}