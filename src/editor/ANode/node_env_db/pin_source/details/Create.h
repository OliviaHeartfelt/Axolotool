#pragma once

#include "../../../../Utility/Utility.h"
#include "Config.h"

namespace NDPinSourceDetails::Create {

    // 0. Source
    inline bool createPinSource(QSqlQuery& query, const NDPinSourceDetails::Config::SourceInfo& sourceInfo) {
        query.prepare(R"(
            INSERT INTO pin_source (id, name)
            VALUES (:id, :name);
        )");
        muuid::uuid newId = muuid::uuid::generate_unix_time_based();

        query.bindValue(":id",   Utility::UUID::uuidToBytes(newId));
        query.bindValue(":name", sourceInfo.name);

        if (!query.exec()) {
            qCritical() << "Failed to insert source:" << query.lastError().text();
            return false;
        }
        return true;
    }

    // 1. Flow
    inline bool createPinFlowSource(QSqlQuery& query, muuid::uuid sourceId, const NDPinSourceDetails::Config::FlowInfo& flowInfo) {
        query.prepare(R"(
            INSERT INTO flow (id, source_id, name, degree)
            VALUES (:id, :source_id, :name, :degree);
        )");
        muuid::uuid newId = muuid::uuid::generate_unix_time_based();

        query.bindValue(":id",        Utility::UUID::uuidToBytes(newId));
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));
        query.bindValue(":name",      flowInfo.name);
        query.bindValue(":degree",    flowInfo.degree);

        if (!query.exec()) {
            qCritical() << "Failed to insert flow source:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createPinFlowSource(QSqlQuery& query, muuid::uuid sourceId, const QList<NDPinSourceDetails::Config::FlowInfo>& flowInfoList) {
        query.prepare(R"(
            INSERT INTO flow (id, source_id, name, degree)
            VALUES (:id, :source_id, :name, :degree);
        )");
        const auto sourceBytesId = Utility::UUID::uuidToBytes(sourceId);

        for (const auto& flowInfo : flowInfoList) {
            muuid::uuid newId = muuid::uuid::generate_unix_time_based();

            query.bindValue(":id",        Utility::UUID::uuidToBytes(newId));
            query.bindValue(":source_id", sourceBytesId);
            query.bindValue(":name",      flowInfo.name);
            query.bindValue(":degree",    flowInfo.degree);

            if (!query.exec()) {
                qCritical() << "Failed to insert flow source:" << query.lastError().text();
                return false;
            }
        }
        return true;
    }

    // 2. Type
    inline bool createPinTypeSource(QSqlQuery& query, muuid::uuid sourceId, const NDPinSourceDetails::Config::TypeInfo& typeInfo) {
        query.prepare(R"(
            INSERT INTO type (id, source_id, name, bit_size)
            VALUES (:id, :source_id, :name, :bit_size);
        )");
        muuid::uuid newId = muuid::uuid::generate_unix_time_based();

        query.bindValue(":id",        Utility::UUID::uuidToBytes(newId));
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));
        query.bindValue(":name",      typeInfo.name);
        query.bindValue(":bit_size",  typeInfo.bit_size);

        if (!query.exec()) {
            qCritical() << "Failed to insert type source:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createPinTypeSource(QSqlQuery& query, muuid::uuid sourceId, const QList<NDPinSourceDetails::Config::TypeInfo>& typeInfoList) {
        query.prepare(R"(
            INSERT INTO type (id, source_id, name, bit_size)
            VALUES (:id, :source_id, :name, :bit_size);
        )");
        const auto sourceBytesId = Utility::UUID::uuidToBytes(sourceId);

        for (const auto& typeInfo : typeInfoList) {
            muuid::uuid newId = muuid::uuid::generate_unix_time_based();

            query.bindValue(":id",        Utility::UUID::uuidToBytes(newId));
            query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));
            query.bindValue(":name",      typeInfo.name);
            query.bindValue(":bit_size",  typeInfo.bit_size);

            if (!query.exec()) {
                qCritical() << "Failed to insert type source:" << query.lastError().text();
                return false;
            }
        }
        return true;
    }

    // 3. Style
    inline bool createPinStyleSource(QSqlQuery& query, muuid::uuid sourceId, const NDPinSourceDetails::Config::StyleInfo& styleInfo) {
        query.prepare(R"(
            INSERT INTO style (id, source_id, name, color, wire_thickness)
            VALUES (:id, :source_id, :name, :color, :wire_thickness);
        )");
        muuid::uuid newId = muuid::uuid::generate_unix_time_based();

        query.bindValue(":id",             Utility::UUID::uuidToBytes(newId));
        query.bindValue(":source_id",      Utility::UUID::uuidToBytes(sourceId));
        query.bindValue(":name",           styleInfo.name);
        query.bindValue(":color",          styleInfo.color);
        query.bindValue(":wire_thickness", styleInfo.wire_thickness);

        if (!query.exec()) {
            qCritical() << "Failed to insert style source:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createPinStyleSource(QSqlQuery& query, muuid::uuid sourceId, const QList<NDPinSourceDetails::Config::StyleInfo>& styleInfoList) {
        query.prepare(R"(
            INSERT INTO style (id, source_id, name, color, wire_thickness)
            VALUES (:id, :source_id, :name, :color, :wire_thickness);
        )");
        const auto sourceBytesId = Utility::UUID::uuidToBytes(sourceId);

        for (const auto& styleInfo : styleInfoList) {
            muuid::uuid newId = muuid::uuid::generate_unix_time_based();

            query.bindValue(":id",             Utility::UUID::uuidToBytes(newId));
            query.bindValue(":source_id",      sourceBytesId);
            query.bindValue(":name",           styleInfo.name);
            query.bindValue(":color",          styleInfo.color);
            query.bindValue(":wire_thickness", styleInfo.wire_thickness);

            if (!query.exec()) {
                qCritical() << "Failed to insert style source:" << query.lastError().text();
                return false;
            }
        }
        return true;
    }
}