#pragma once

#include "../../../../Utility/Utility.h"
#include "Config.h"

namespace NDPinSourceDetails::Create {

    // 0. Source
    inline std::optional<muuid::uuid> createPinSource(QSqlQuery& query, const NDPinSourceDetails::Config::SourceInfo& sourceInfo) {
        query.prepare(R"(
            INSERT INTO pin_source (id, name)
            VALUES (:id, :name);
        )");
        muuid::uuid newId = muuid::uuid::generate_unix_time_based();

        query.bindValue(":id", Utility::UUID::uuidToBytes(newId));
        query.bindValue(":name", sourceInfo.name);

        if (!query.exec()) {
            qCritical() << "Failed to insert source:" << query.lastError().text();
            return std::nullopt;
        }
        return newId;
    }
    inline std::optional<muuid::uuid> createPinContributor(QSqlQuery& query, const NDPinSourceDetails::Config::ContributorInfo& contributorInfo) {
        query.prepare(R"(
            INSERT INTO pin_contributor (id, source_id, name)
            VALUES (:id, :source_id, :name);
        )");
        muuid::uuid newId = muuid::uuid::generate_unix_time_based();

        query.bindValue(":id", Utility::UUID::uuidToBytes(newId));
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(contributorInfo.sourceId));
        query.bindValue(":name", contributorInfo.name);

        if (!query.exec()) {
            qCritical() << "Failed to insert contributor:" << query.lastError().text();
            return std::nullopt;
        }
        return newId;
    }

    // 1. Flow
    inline std::optional<muuid::uuid> createPinFlowSource(QSqlQuery& query, muuid::uuid contributorId, const NDPinSourceDetails::Config::FlowInfo& flowInfo) {
        query.prepare(R"(
            INSERT INTO flow (id, contributor_id, name, degree)
            VALUES (:id, :contributor_id, :name, :degree);
        )");
        muuid::uuid newId = muuid::uuid::generate_unix_time_based();

        query.bindValue(":id", Utility::UUID::uuidToBytes(newId));
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));
        query.bindValue(":name", flowInfo.name);
        query.bindValue(":degree", flowInfo.degree);

        if (!query.exec()) {
            qCritical() << "Failed to insert flow source:" << query.lastError().text();
            return std::nullopt;
        }
        return newId;
    }

    // 2. Type
    inline std::optional<muuid::uuid> createPinTypeSource(QSqlQuery& query, muuid::uuid contributorId, const NDPinSourceDetails::Config::TypeInfo& typeInfo) {
        query.prepare(R"(
            INSERT INTO type (id, contributor_id, name, bit_size)
            VALUES (:id, :contributor_id, :name, :bit_size);
        )");
        muuid::uuid newId = muuid::uuid::generate_unix_time_based();

        query.bindValue(":id", Utility::UUID::uuidToBytes(newId));
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));
        query.bindValue(":name", typeInfo.name);
        query.bindValue(":bit_size", typeInfo.bit_size);

        if (!query.exec()) {
            qCritical() << "Failed to insert type source:" << query.lastError().text();
            return std::nullopt;
        }
        return newId;
    }

    // 3. Style
    inline std::optional<muuid::uuid> createPinStyleSource(QSqlQuery& query, muuid::uuid contributorId, const NDPinSourceDetails::Config::StyleInfo& styleInfo) {
        query.prepare(R"(
            INSERT INTO style (id, contributor_id, name, color, wire_thickness)
            VALUES (:id, :contributor_id, :name, :color, :wire_thickness);
        )");
        muuid::uuid newId = muuid::uuid::generate_unix_time_based();

        query.bindValue(":id", Utility::UUID::uuidToBytes(newId));
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));
        query.bindValue(":name", styleInfo.name);
        query.bindValue(":color", styleInfo.color);
        query.bindValue(":wire_thickness", styleInfo.wire_thickness);

        if (!query.exec()) {
            qCritical() << "Failed to insert style source:" << query.lastError().text();
            return std::nullopt;
        }
        return newId;
    }
}