#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "Config.h"

namespace NDWireSourceDetails::Create {

    inline bool createWireSource(QSqlQuery& query, const NDWireSourceDetails::Config::CreateWireSourceRecord& newSource) {
        query.prepare(R"(
            INSERT INTO wire_source (id,  global_source_id,  name)
            VALUES (                :id, :global_source_id, :name);
        )");

        query.bindValue(":id",               Utility::UUID::uuidToBytes(newSource.id));
        query.bindValue(":global_source_id", newSource.globalSourceId ? QVariant(Utility::UUID::uuidToBytes(*newSource.globalSourceId)) : QVariant());
        query.bindValue(":name",             newSource.name);

        if (!query.exec()) {
            qCritical() << "Failed to insert wire source:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createWireContributor(QSqlQuery& query, const NDWireSourceDetails::Config::CreateWireContributorRecord& newContributor) {
        query.prepare(R"(
            INSERT INTO wire_contributor (id,  source_id,  name)
            VALUES (                     :id, :source_id, :name);
        )");

        query.bindValue(":id",        Utility::UUID::uuidToBytes(newContributor.id));
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(newContributor.sourceId));
        query.bindValue(":name",      newContributor.name);

        if (!query.exec()) {
            qCritical() << "Failed to insert wire contributor:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createWireStyle(QSqlQuery& query, const NDWireSourceDetails::Config::CreateWireStyleRecord& newStyle) {
        query.prepare(R"(
            INSERT INTO wire_style (id,  contributor_id,  name,  color,  wire_thickness,  metadata)
            VALUES (               :id, :contributor_id, :name, :color, :wire_thickness, :metadata);
        )");

        query.bindValue(":id",             Utility::UUID::uuidToBytes(newStyle.id));
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(newStyle.contributorId));

        query.bindValue(":name",           newStyle.name);
        query.bindValue(":color",          newStyle.color.rgba());
        query.bindValue(":wire_thickness", newStyle.wireThickness);
        query.bindValue(":metadata",       newStyle.metadata ? QVariant(Utility::ByteArray::toQByteArray(*newStyle.metadata)) : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to insert wire style:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createWireData(QSqlQuery& query, const NDWireSourceDetails::Config::CreateWireDataRecord& newData) {
        query.prepare(R"(
            INSERT INTO wire_data (id,  contributor_id,  name,  color,  wire_thickness,  metadata)
            VALUES (              :id, :contributor_id, :name, :color, :wire_thickness, :metadata);
        )");

        query.bindValue(":id",             Utility::UUID::uuidToBytes(newData.id));
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(newData.contributorId));

        query.bindValue(":name", newData.name);
        query.bindValue(":data", newData.data ? QVariant(Utility::ByteArray::toQByteArray(*newData.data)) : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to insert wire data:" << query.lastError().text();
            return false;
        }
        return true;
    }
}