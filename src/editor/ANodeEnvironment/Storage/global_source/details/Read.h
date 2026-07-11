#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "../../NDHelpers.h"
#include "Config.h"

namespace NDGlobalSourceDetails::Read {

    inline std::optional<NDGlobalSourceDetails::Config::FullGlobalSourceRecord> getGlobalSource(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT name, dsc
            FROM global_source 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getGlobalSource query:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        std::optional<QString> dsc = std::nullopt;
        if (const auto _dsc = query.value(2); !_dsc.isNull())
            dsc = _dsc.toString();

        return NDGlobalSourceDetails::Config::FullGlobalSourceRecord{
            id,
            query.value(0).toString(),
            dsc
        };
    }
    inline std::optional<QList<NDGlobalSourceDetails::Config::FullGlobalSourceRecord>> getAllGlobalSources(QSqlQuery& query, const bool continueAtFail = false) {
        QList<NDGlobalSourceDetails::Config::FullGlobalSourceRecord> list;
        
        query.prepare(R"(
            SELECT id, name, dsc
            FROM global_source
        )");

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllGlobalSources query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {

            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());

            if (!id) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            std::optional<QString> dsc = std::nullopt;
            if (const auto _dsc = query.value(2); !_dsc.isNull())
                dsc = _dsc.toString();

            list.append(NDGlobalSourceDetails::Config::FullGlobalSourceRecord{
                *id,
                query.value(0).toString(),
                dsc
            });
        }
        return list;
    }
}