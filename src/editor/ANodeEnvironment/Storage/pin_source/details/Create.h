#pragma once

#include "../../../../Utility/Utility.h"
#include "Config.h"

namespace NDPinSourceDetails::Create {

    // 0. Source
    inline bool createPinSource(QSqlQuery& query, const NDPinSourceDetails::Config::CreatePinSourceRecord& newSource) {
        query.prepare(R"(
            INSERT INTO pin_source (id,  global_source_id,  name)
            VALUES (               :id, :global_source_id, :name);
        )");

        query.bindValue(":id",               Utility::UUID::uuidToBytes(newSource.id));
        query.bindValue(":global_source_id", newSource.globalSourceId ? QVariant(Utility::UUID::uuidToBytes(*newSource.globalSourceId)) : QVariant());
        query.bindValue(":name",             newSource.name);

        if (!query.exec()) {
            qCritical() << "Failed to insert source:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createPinContributor(QSqlQuery& query, const NDPinSourceDetails::Config::CreatePinContributorRecord& newContributor) {
        query.prepare(R"(
            INSERT INTO pin_contributor (id,  source_id,  name)
            VALUES (                    :id, :source_id, :name);
        )");

        query.bindValue(":id",        Utility::UUID::uuidToBytes(newContributor.id));
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(newContributor.sourceId));
        query.bindValue(":name",      newContributor.name);

        if (!query.exec()) {
            qCritical() << "Failed to insert contributor:" << query.lastError().text();
            return false;
        }
        return true;
    }

    // 1. Flow
    inline bool createPinFlowSource(QSqlQuery& query, const NDPinSourceDetails::Config::CreatePinFlowRecord& newFlow) {
        query.prepare(R"(
            INSERT INTO pin_flow (id,  contributor_id,  name,  degree)
            VALUES (             :id, :contributor_id, :name, :degree);
        )");

        query.bindValue(":id",             Utility::UUID::uuidToBytes(newFlow.id));
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(newFlow.contributorId));
        query.bindValue(":name",           newFlow.name);
        query.bindValue(":degree",         newFlow.degree);

        if (!query.exec()) {
            qCritical() << "Failed to insert flow source:" << query.lastError().text();
            return false;
        }
        return true;
    }

    // 2. Type
    inline bool createPinTypeSource(QSqlQuery& query, const NDPinSourceDetails::Config::CreatePinTypeRecord& newtype) {
        query.prepare(R"(
            INSERT INTO pin_type (id,  contributor_id,  name,  bit_size)
            VALUES (             :id, :contributor_id, :name, :bit_size);
        )");

        query.bindValue(":id",             Utility::UUID::uuidToBytes(newtype.id));
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(newtype.contributorId));
        query.bindValue(":name",           newtype.name);
        query.bindValue(":bit_size",       newtype.bit_size);

        if (!query.exec()) {
            qCritical() << "Failed to insert type source:" << query.lastError().text();
            return false;
        }
        return true;
    }

    // 3. Style
    inline bool createPinStyleSource(QSqlQuery& query, const NDPinSourceDetails::Config::CreatePinStyleRecord& newStyle) {
        query.prepare(R"(
            INSERT INTO pin_style (id,  contributor_id,  name,  color,  wire_thickness)
            VALUES (              :id, :contributor_id, :name, :color, :wire_thickness);
        )");

        query.bindValue(":id",             Utility::UUID::uuidToBytes(newStyle.id));
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(newStyle.contributorId));
        query.bindValue(":name",           newStyle.name);
        query.bindValue(":color",          newStyle.color.rgba());
        query.bindValue(":wire_thickness", newStyle.wireThickness);

        if (!query.exec()) {
            qCritical() << "Failed to insert style source:" << query.lastError().text();
            return false;
        }
        return true;
    }
}