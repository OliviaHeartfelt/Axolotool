#pragma once

#include "../../../../Utility/Utility.h"

namespace NDPinSourceDetails::Update {

    // 0. Source
    inline bool updateSource(QSqlQuery& query, const muuid::uuid& id, const NDPinSourceDetails::Config::UpdatePinSourceRecord& newSource) {
        QStringList clauses;

        if (newSource.name) clauses.append("name = :name");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE pin_source SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newSource.name) query.bindValue(":name", *newSource.name);

        if (!query.exec()) {
            qWarning() << "Failed to update source:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool updateContributor(QSqlQuery& query, const muuid::uuid& id, const NDPinSourceDetails::Config::UpdatePinContributorRecord& newContributor) {
        QStringList clauses;

        if (newContributor.name) clauses.append("name = :name");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE pin_contributor SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

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

        if (newFlow.name)   clauses.append("name = :name");
        if (newFlow.degree) clauses.append("degree = :degree");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE pin_flow SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

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

        if (newType.name)     clauses.append("name = :name");
        if (newType.bit_size) clauses.append("bit_size = :bit_size");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE pin_type SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

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

        if (newStyle.name)           clauses.append("name = :name");
        if (newStyle.color)          clauses.append("color = :color");
        if (newStyle.wire_thickness) clauses.append("wire_thickness = :wire_thickness");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE pin_style SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newStyle.name)           query.bindValue(":name",           *newStyle.name);
        if (newStyle.color)          query.bindValue(":color",          newStyle.color->rgba());
        if (newStyle.wire_thickness) query.bindValue(":wire_thickness", *newStyle.wire_thickness);

        if (!query.exec()) {
            qWarning() << "Failed to update type style:" << query.lastError().text();
            return false;
        }
        return true;
    }
}