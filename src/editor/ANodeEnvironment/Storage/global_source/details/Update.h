#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "Config.h"

namespace NDGlobalSourceDetails::Update {

    inline bool updateGlobalSource(QSqlQuery& query, muuid::uuid id, const NDGlobalSourceDetails::Config::UpdateGlobalSourceRecord& newProperties) {
        QStringList clauses;

        if (newProperties.id)   clauses.append("id = :new_id");
        if (newProperties.name) clauses.append("name = :name");

        const auto* optPtr = std::get_if<std::optional<QString>>(&newProperties.dsc);
        if (optPtr) clauses.append("dsc = :dsc");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE global_source SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.name)  query.bindValue(":name", newProperties.name ? *newProperties.name : QVariant());
        if (newProperties.name)  query.bindValue(":name", *newProperties.name);

        if (optPtr) query.bindValue(":dsc", optPtr->has_value() ? optPtr->value() : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update global source:" << query.lastError().text();
            return false;
        }
        return true;
    }
}