#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "Config.h"

namespace NDNodeSourceDetails::Create {

    inline bool createNodeSource(QSqlQuery& query, const NDNodeSourceDetails::Config::CreateNodeSourceRecord& newNodeSource) {
        query.prepare(R"(
            INSERT INTO node_source (id,  name)
            VALUES (                :id, :name);
        )");

        query.bindValue(":id",   Utility::UUID::uuidToBytes(newNodeSource.id));
        query.bindValue(":name", newNodeSource.name);

        if (!query.exec()) {
            qWarning() << "Failed to execute create node source:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createNodeContributor(QSqlQuery& query, const NDNodeSourceDetails::Config::CreateNodeContributorRecord& newNodeContributor) {
        query.prepare(R"(
            INSERT INTO node_contributor (id,  source_id,  name)
            VALUES (                     :id, :source_id, :name);
        )");

        query.bindValue(":id",        Utility::UUID::uuidToBytes(newNodeContributor.id));
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(newNodeContributor.sourceId));

        query.bindValue(":name", newNodeContributor.name);

        if (!query.exec()) {
            qWarning() << "Failed to execute create node contributor:" << query.lastError().text();
            return false;
        }
        return true;
    }
    template<NDConcepts::ByteConvertible Metadata>
    inline bool createNodeType(QSqlQuery& query, const NDNodeSourceDetails::Config::CreateNodeTypeRecord<Metadata>& newNodeType) {
        query.prepare(R"(
            INSERT INTO node_contributor (id,  contributor_id,  name,  metadata)
            VALUES (                     :id, :contributor_id, :name, :metadata);
        )");

        query.bindValue(":id",             Utility::UUID::uuidToBytes(newNodeType.id));
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(newNodeType.sourceId));

        query.bindValue(":name",     newNodeType.name);
        query.bindValue(":metadata", newNodeType.metadata ? QVariant(newNodeType.metadata->classToByteArray()) : QVariant());

        if (!query.exec()) {
            qWarning() << "Failed to execute create node type:" << query.lastError().text();
            return false;
        }
        return true;
    }
    template<NDConcepts::ByteConvertible Data>
    inline bool createNodeData(QSqlQuery& query, const NDNodeSourceDetails::Config::CreateNodeDataRecord<Data>& newNodeData) {
        query.prepare(R"(
            INSERT INTO node_contributor (id,  contributor_id,  name,  data)
            VALUES (                     :id, :contributor_id, :name, :data);
        )");

        query.bindValue(":id",             Utility::UUID::uuidToBytes(newNodeData.id));
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(newNodeData.sourceId));

        query.bindValue(":name", newNodeData.name);
        query.bindValue(":data", newNodeData.metadata ? QVariant(newNodeData.data->classToByteArray()) : QVariant());

        if (!query.exec()) {
            qWarning() << "Failed to execute create node data:" << query.lastError().text();
            return false;
        }
        return true;
    }
}

