#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "Config.h"

namespace NDGlobalSourceDetails::Create {

    inline bool createGlobalSource(QSqlQuery& query, const NDGlobalSourceDetails::Config::CreateGlobalSourceRecord& newSource) {
        query.prepare(R"(
            INSERT INTO global_source (id,  name,  dsc)
            VALUES (                  :id, :name, :dsc);
        )");

        query.bindValue(":id",   Utility::UUID::uuidToBytes(newSource.id));
        query.bindValue(":name", newSource.name);
        query.bindValue(":dsc",  newSource.dsc ? *newSource.dsc : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to insert global source:" << query.lastError().text();
            return false;
        }
        return true;
    }
}