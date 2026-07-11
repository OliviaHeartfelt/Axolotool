#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "Config.h"

namespace NDNodeSourceDetails::Update {

    inline bool updateNodeSource(QSqlQuery& query, const muuid::uuid& id, const NDNodeSourceDetails::Config::UpdateNodeSourceRecord& newProperties) {
        QStringList clauses;

        if (newProperties.name)   clauses.append("name = :name");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE node_source SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.name) query.bindValue(":name", *newProperties.name);

        if (!query.exec()) {
            qWarning() << "Failed to update node source:" << query.lastError().text();
            return false;
        }
        return true;
    }

    inline bool updateNodeContributor(QSqlQuery& query, const muuid::uuid& id, const NDNodeSourceDetails::Config::UpdateNodeContributorRecord& newProperties) {
        QStringList clauses;

        if (newProperties.name)   clauses.append("name = :name");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE node_contributor SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.name) query.bindValue(":name", *newProperties.name);

        if (!query.exec()) {
            qWarning() << "Failed to update node contributor:" << query.lastError().text();
            return false;
        }
        return true;
    }

    template<NDConcepts::ByteConvertible Metadata>
    inline bool updateNodeType(QSqlQuery& query, const muuid::uuid& id, const NDNodeSourceDetails::Config::UpdateNodeTypeRecord<Metadata>& newProperties) {
        QStringList clauses;

        if (newProperties.name)   clauses.append("name = :name");

        const auto* optPtr = std::get_if<std::optional<Metadata>>(&newProperties.metadata);
        if (optPtr) {
            clauses.append("metadata = :metadata");
        }

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE node_type SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.name) query.bindValue(":name", *newProperties.name);

        if (optPtr) {
            if (optPtr->has_value())
                query.bindValue(":metadata", QVariant(optPtr->value().classToByteArray()));
            else
                query.bindValue(":metadata", QVariant(QMetaType::fromType<QByteArray>()));
        }

        if (!query.exec()) {
            qWarning() << "Failed to update node type:" << query.lastError().text();
            return false;
        }
        return true;
    }

    template<NDConcepts::ByteConvertible Data>
    inline bool updateNodeData(QSqlQuery& query, const muuid::uuid& id, const NDNodeSourceDetails::Config::UpdateNodeDataRecord<Data>& newProperties) {
        QStringList clauses;

        if (newProperties.name)   clauses.append("name = :name");

        const auto* optPtr = std::get_if<std::optional<Data>>(&newProperties.data);
        if (optPtr) {
            clauses.append("data = :data");
        }

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE node_data SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.name) query.bindValue(":name", *newProperties.name);

        if (optPtr) {
            if (optPtr->has_value())
                query.bindValue(":data", QVariant(optPtr->value().classToByteArray()));
            else
                query.bindValue(":data", QVariant(QMetaType::fromType<QByteArray>()));
        }

        if (!query.exec()) {
            qWarning() << "Failed to update node data:" << query.lastError().text();
            return false;
        }
        return true;
    }
}