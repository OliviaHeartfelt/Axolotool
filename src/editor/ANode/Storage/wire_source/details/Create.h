#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "Config.h"

namespace NDWireSourceDetails::Create {

    inline bool createWireSource(QSqlQuery& query, const NDWireSourceDetails::Config::CreateWireSourceRecord& newSource) {
        query.prepare(R"(
            INSERT INTO wire_source (id,  name)
            VALUES (                :id, :name);
        )");

        query.bindValue(":id",   Utility::UUID::uuidToBytes(newSource.id));
        query.bindValue(":name", newSource.name);

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
    template<NDConcepts::ByteConvertible Metadata>
    inline bool createWireStyle(QSqlQuery& query, const NDWireSourceDetails::Config::CreateWireStyleRecord<Metadata>& newStyle) {
        query.prepare(R"(
            INSERT INTO wire_style (id,  contributor_id,  name,  color,  wire_thickness,  metadata)
            VALUES (               :id, :contributor_id, :name, :color, :wire_thickness, :metadata);
        )");

        query.bindValue(":id",             Utility::UUID::uuidToBytes(newStyle.id));
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(newStyle.sourceId));

        query.bindValue(":name",           newStyle.name);
        query.bindValue(":color",          newStyle.color.rgba());
        query.bindValue(":wire_thickness", newStyle.wireThickness);
        query.bindValue(":metadata",       newStyle.metadata ? QVariant(newStyle.metadata->classToByteArray()) : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to insert wire style:" << query.lastError().text();
            return false;
        }
        return true;
    }
    template<NDConcepts::ByteConvertible Data>
    inline bool createWireData(QSqlQuery& query, const NDWireSourceDetails::Config::CreateWireDataRecord<Data>& newData) {
        query.prepare(R"(
            INSERT INTO wire_data (id,  contributor_id,  name,  color,  wire_thickness,  metadata)
            VALUES (              :id, :contributor_id, :name, :color, :wire_thickness, :metadata);
        )");

        query.bindValue(":id",             Utility::UUID::uuidToBytes(newData.id));
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(newData.sourceId));

        query.bindValue(":name", newData.name);
        query.bindValue(":data", newData.data ? QVariant(newData.data->classToByteArray()) : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to insert wire data:" << query.lastError().text();
            return false;
        }
        return true;
    }
}