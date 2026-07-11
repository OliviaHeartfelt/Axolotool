#pragma once

#include "../../../../Utility/Utility.h"

namespace NDPinDetails::Update {

    inline bool updatePin(QSqlQuery& query, const muuid::uuid& id, const NDPinDetails::Config::UpdatePinRecord& newProperties) {
        QStringList clauses;

        if (std::holds_alternative<std::optional<muuid::uuid>>(newProperties.flowId))  clauses.append("flow_id = :flow_id");
        if (std::holds_alternative<std::optional<muuid::uuid>>(newProperties.typeId))  clauses.append("type_id = :type_id");
        if (std::holds_alternative<std::optional<muuid::uuid>>(newProperties.styleId)) clauses.append("style_id = :style_id");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE pin SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        using UpdateField = std::variant<std::monostate, std::optional<muuid::uuid>>;
        auto unpackUpdateField = [](const UpdateField& field) -> QVariant {
            if (const auto* opt = std::get_if<std::optional<muuid::uuid>>(&field)) {
                return *opt ? Utility::UUID::uuidToBytes(**opt) : QVariant(QVariant::fromValue(nullptr));
            }
            return QVariant();
        };

        if (std::holds_alternative<std::optional<muuid::uuid>>(newProperties.flowId)) {
            query.bindValue(":flow_id", unpackUpdateField(newProperties.flowId));
        }
        if (std::holds_alternative<std::optional<muuid::uuid>>(newProperties.typeId)) {
            query.bindValue(":type_id", unpackUpdateField(newProperties.typeId));
        }
        if (std::holds_alternative<std::optional<muuid::uuid>>(newProperties.styleId)) {
            query.bindValue(":style_id", unpackUpdateField(newProperties.styleId));
        }

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update pin:" << query.lastError().text();
            return false;
        }
        return true;
    }
}