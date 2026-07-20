#pragma once

#include "../../../../Utility/Utility.h"

namespace NDPinSourceDetails::Update {

    // 0. Source
    inline bool updateSource(QSqlQuery& query, const muuid::uuid& id, const NDPinSourceDetails::Config::UpdatePinSourceRecord& newSource) {
        QStringList clauses;

        if (newSource.id)             clauses.append("id = :new_id");
        if (newSource.globalSourceId) clauses.append("global_source_id = :new_global_source_id");

        if (newSource.name) clauses.append("name = :name");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE pin_source SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newSource.id)             query.bindValue(":new_id",               Utility::UUID::uuidToBytes(*newSource.id));
        if (newSource.globalSourceId) query.bindValue(":new_global_source_id", Utility::UUID::uuidToBytes(*newSource.globalSourceId));

        if (newSource.name) query.bindValue(":name", *newSource.name);

        if (!query.exec()) {
            qWarning() << "Failed to update source:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool updateContributor(QSqlQuery& query, const muuid::uuid& id, const NDPinSourceDetails::Config::UpdatePinContributorRecord& newContributor) {
        QStringList clauses;

        if (newContributor.id)       clauses.append("id = :new_id");
        if (newContributor.sourceId) clauses.append("source_id = :new_source_id");

        if (newContributor.name) clauses.append("name = :name");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE pin_contributor SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newContributor.id)       query.bindValue(":new_id",        Utility::UUID::uuidToBytes(*newContributor.id));
        if (newContributor.sourceId) query.bindValue(":new_source_id", Utility::UUID::uuidToBytes(*newContributor.sourceId));

        if (newContributor.name)   query.bindValue(":name", *newContributor.name);

        if (!query.exec()) {
            qWarning() << "Failed to update contributor:" << query.lastError().text();
            return false;
        }
        return true;
    }

    // 1. Flow
    inline bool updateFlow(QSqlQuery& query, const muuid::uuid& id, const NDPinSourceDetails::Config::UpdatePinFlowRecord& newFlow) {
        QStringList clauses;

        if (newFlow.id)            clauses.append("id = :new_id");
        if (newFlow.contributorId) clauses.append("contributor_id = :new_contributor_id");

        if (newFlow.name)   clauses.append("name = :name");
        if (newFlow.degree) clauses.append("degree = :degree");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE pin_flow SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newFlow.id)            query.bindValue(":new_id",             Utility::UUID::uuidToBytes(*newFlow.id));
        if (newFlow.contributorId) query.bindValue(":new_contributor_id", Utility::UUID::uuidToBytes(*newFlow.contributorId));


        if (newFlow.name)   query.bindValue(":name",   *newFlow.name);
        if (newFlow.degree) query.bindValue(":degree", *newFlow.degree);

        if (!query.exec()) {
            qWarning() << "Failed to update flow:" << query.lastError().text();
            return false;
        }
        return true;
    }

    // 2. Type
    inline bool updateType(QSqlQuery& query, const muuid::uuid& id, const NDPinSourceDetails::Config::UpdatePinTypeRecord& newType) {
        QStringList clauses;

        if (newType.id)            clauses.append("id = :new_id");
        if (newType.contributorId) clauses.append("contributor_id = :new_contributor_id");

        if (newType.name)     clauses.append("name = :name");
        if (newType.bit_size) clauses.append("bit_size = :bit_size");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE pin_type SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newType.id)            query.bindValue(":new_id",             Utility::UUID::uuidToBytes(*newType.id));
        if (newType.contributorId) query.bindValue(":new_contributor_id", Utility::UUID::uuidToBytes(*newType.contributorId));

        if (newType.name)     query.bindValue(":name",     *newType.name);
        if (newType.bit_size) query.bindValue(":bit_size", *newType.bit_size);

        if (!query.exec()) {
            qWarning() << "Failed to update type:" << query.lastError().text();
            return false;
        }
        return true;
    }

    // 3. Style
    inline bool updateStyle(QSqlQuery& query, const muuid::uuid& id, const NDPinSourceDetails::Config::UpdatePinStyleRecord& newStyle) {
        QStringList clauses;

        if (newStyle.id)            clauses.append("id = :new_id");
        if (newStyle.contributorId) clauses.append("contributor_id = :new_contributor_id");

        if (newStyle.name)           clauses.append("name = :name");
        if (newStyle.color)          clauses.append("color = :color");
        if (newStyle.wireThickness)  clauses.append("wire_thickness = :wire_thickness");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE pin_style SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newStyle.id)            query.bindValue(":new_id",             Utility::UUID::uuidToBytes(*newStyle.id));
        if (newStyle.contributorId) query.bindValue(":new_contributor_id", Utility::UUID::uuidToBytes(*newStyle.contributorId));

        if (newStyle.name)           query.bindValue(":name",           *newStyle.name);
        if (newStyle.color)          query.bindValue(":color",          newStyle.color->rgba());
        if (newStyle.wireThickness)  query.bindValue(":wire_thickness", *newStyle.wireThickness);

        if (!query.exec()) {
            qWarning() << "Failed to update type style:" << query.lastError().text();
            return false;
        }
        return true;
    }
}