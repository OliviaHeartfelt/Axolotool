#pragma once

#include "../../../../Utility/Utility.h"
#include "Config.h"

namespace NDWidgetSourceDetails::Create {

    inline std::optional<muuid::uuid> createWidgetSource(QSqlQuery& query, const NDWidgetSourceDetails::Config::CreateWidgetSourceRecord& newSource) {
        muuid::uuid id = muuid::uuid::generate_unix_time_based();
        query.prepare(R"(
            INSERT INTO widget_source (id, name)
            VALUES (:id, :name);
        )");

        query.bindValue(":id",   Utility::UUID::uuidToBytes(id));
        query.bindValue(":name", newSource.name);

        if (!query.exec()) {
            qCritical() << "Failed to insert widget source:" << query.lastError().text();
            return std::nullopt;
        }
        return id;
    }
    inline std::optional<muuid::uuid> createWidgetContributor(QSqlQuery& query, const NDWidgetSourceDetails::Config::CreateWidgetContributorRecord& newContributor) {
        muuid::uuid id = muuid::uuid::generate_unix_time_based();
        query.prepare(R"(
            INSERT INTO widget_contributor (id, source_id, name)
            VALUES (:id, :source_id, :name);
        )");

        query.bindValue(":id",        Utility::UUID::uuidToBytes(id));
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(newContributor.sourceId));
        query.bindValue(":name",      newContributor.name);

        if (!query.exec()) {
            qCritical() << "Failed to insert widget contributor:" << query.lastError().text();
            return std::nullopt;
        }
        return id;
    }
    template<NDWidgetSourceDetails::Config::ByteConvertible Metadata>
    inline std::optional<muuid::uuid> createWidgetType(QSqlQuery& query, const NDWidgetSourceDetails::Config::CreateWidgetTypeRecord<Metadata>& newType) {
        muuid::uuid id = muuid::uuid::generate_unix_time_based();
        query.prepare(R"(
            INSERT INTO widget_type (id, contributor_id, name, metadata)
            VALUES (:id, :contributor_id, :name, :metadata);
        )");

        query.bindValue(":id",             Utility::UUID::uuidToBytes(id));
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(newType.contributorId));
        query.bindValue(":name",           newType.name);
        query.bindValue(":metadata",       newType.metadata ? QVariant(newType.metadata->classToByteArray()) : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to insert widget type:" << query.lastError().text();
            return std::nullopt;
        }
        return id;
    }
    template<NDWidgetSourceDetails::Config::ByteConvertible Data>
    inline std::optional<muuid::uuid> createWidgetData(QSqlQuery& query, const NDWidgetSourceDetails::Config::CreateWidgetDataRecord<Data>& newData) {
        muuid::uuid id = muuid::uuid::generate_unix_time_based();
        query.prepare(R"(
            INSERT INTO widget_data (id, contributor_id, name, metadata)
            VALUES (:id, :contributor_id, :name, :metadata);
        )");

        query.bindValue(":id",             Utility::UUID::uuidToBytes(id));
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(newData.contributorId));
        query.bindValue(":name",           newData.name);
        query.bindValue(":metadata",       newData.data ? QVariant(newData.data->classToByteArray()) : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to insert widget data:" << query.lastError().text();
            return std::nullopt;
        }
        return id;
    }
}