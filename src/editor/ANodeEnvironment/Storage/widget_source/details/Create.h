#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "Config.h"

namespace NDWidgetSourceDetails::Create {

    inline bool createWidgetSource(QSqlQuery& query, const NDWidgetSourceDetails::Config::CreateWidgetSourceRecord& newSource) {
        query.prepare(R"(
            INSERT INTO widget_source (id,  global_source_id,  name)
            VALUES (                  :id, :global_source_id, :name);
        )");

        query.bindValue(":id",               Utility::UUID::uuidToBytes(newSource.id));
        query.bindValue(":global_source_id", newSource.globalSourceId ? QVariant(Utility::UUID::uuidToBytes(*newSource.globalSourceId)) : QVariant());
        query.bindValue(":name",             newSource.name);

        if (!query.exec()) {
            qCritical() << "Failed to insert widget source:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createWidgetContributor(QSqlQuery& query, const NDWidgetSourceDetails::Config::CreateWidgetContributorRecord& newContributor) {
        query.prepare(R"(
            INSERT INTO widget_contributor (id,  source_id,  name)
            VALUES (                       :id, :source_id, :name);
        )");

        query.bindValue(":id",        Utility::UUID::uuidToBytes(newContributor.id));
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(newContributor.sourceId));
        query.bindValue(":name",      newContributor.name);

        if (!query.exec()) {
            qCritical() << "Failed to insert widget contributor:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createWidgetType(QSqlQuery& query, const NDWidgetSourceDetails::Config::CreateWidgetTypeRecord& newType) {
        query.prepare(R"(
            INSERT INTO widget_type (id,  contributor_id,  name,  metadata)
            VALUES (                :id, :contributor_id, :name, :metadata);
        )");

        query.bindValue(":id",             Utility::UUID::uuidToBytes(newType.id));
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(newType.contributorId));
        query.bindValue(":name",           newType.name);
        query.bindValue(":metadata",       newType.metadata ? QVariant(Utility::ByteArray::toQByteArray(*newType.metadata)) : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to insert widget type:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createWidgetData(QSqlQuery& query, const NDWidgetSourceDetails::Config::CreateWidgetDataRecord& newData) {
        query.prepare(R"(
            INSERT INTO widget_data (id,  contributor_id,  name,  data)
            VALUES (                :id, :contributor_id, :name, :data);
        )");

        query.bindValue(":id",             Utility::UUID::uuidToBytes(newData.id));
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(newData.contributorId));
        query.bindValue(":name",           newData.name);
        query.bindValue(":data",           newData.data ? QVariant(Utility::ByteArray::toQByteArray(*newData.data)) : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to insert widget data:" << query.lastError().text();
            return false;
        }
        return true;
    }
}