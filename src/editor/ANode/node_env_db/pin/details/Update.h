#pragma once

#include "../../../../Utility/Utility.h"

namespace NDPinDetails::Update {

    struct Property {
        const bool modify = false;
        const std::optional<muuid::uuid> value = std::nullopt;
    };
    inline bool updatePin(
        QSqlQuery& query,
        const muuid::uuid& id,
        const Property& flow,
        const Property& type,
        const Property& style
    ) {
        QStringList clauses;

        if (flow.modify)  clauses.append(flow.value  ? "flow_id = :flow_id"   : "flow_id = NULL");
        if (type.modify)  clauses.append(type.value  ? "type_id = :type_id"   : "type_id = NULL");
        if (style.modify) clauses.append(style.value ? "style_id = :style_id" : "style_id = NULL");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE pin SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (flow.modify  && flow.value)  query.bindValue(":flow_id",  Utility::UUID::uuidToBytes(*flow.value));
        if (type.modify  && type.value)  query.bindValue(":type_id",  Utility::UUID::uuidToBytes(*type.value));
        if (style.modify && style.value) query.bindValue(":style_id", Utility::UUID::uuidToBytes(*style.value));

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update pin:" << query.lastError().text();
            return false;
        }
        return true;
    }
}