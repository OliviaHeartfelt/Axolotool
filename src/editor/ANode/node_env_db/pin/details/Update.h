#pragma once

#include "../../../../Utility/Utility.h"

namespace NDPinDetails::Update {

    struct Property {
        bool modify = false;
        std::optional<muuid::uuid> value = std::nullopt;
    };
    struct UpdateProperties {
        Property flow;
        Property type;
        Property style;
    };
    inline bool updatePin(
        QSqlQuery& query,
        const muuid::uuid& id,
        const UpdateProperties& newProperties
    ) {
        QStringList clauses;

        if (newProperties.flow.modify)  clauses.append("flow_id = :flow_id");
        if (newProperties.type.modify)  clauses.append("type_id = :type_id");
        if (newProperties.style.modify) clauses.append("style_id = :style_id");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE pin SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.flow.modify  && newProperties.flow.value)  query.bindValue(":flow_id", 
            newProperties.flow.value ?  Utility::UUID::uuidToBytes(*newProperties.flow.value) : QVariant());

        if (newProperties.type.modify && newProperties.type.value)  query.bindValue(":type_id", 
            newProperties.type.value ?  Utility::UUID::uuidToBytes(*newProperties.type.value) : QVariant());

        if (newProperties.style.modify && newProperties.style.value) query.bindValue(":style_id", 
            newProperties.style.value ? Utility::UUID::uuidToBytes(*newProperties.style.value) : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update pin:" << query.lastError().text();
            return false;
        }
        return true;
    }
}