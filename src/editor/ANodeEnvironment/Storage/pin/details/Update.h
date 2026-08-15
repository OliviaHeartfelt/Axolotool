#pragma once

#include "../../../../Utility/Utility.h"

namespace NDPinDetails::Update {

    inline bool updatePinCore(QSqlQuery& query, const muuid::uuid& id, const NDPinDetails::Config::UpdatePinCoreRecord& newProperties) {
        QStringList clauses;

        if (newProperties.id)              clauses.append("id = :new_id");
        if (newProperties.contributorId)   clauses.append("contributor_id = :new_contributor_id");
        if (newProperties.visualFactoryId) clauses.append("visual_factory_id = :new_visual_factory_id");

        const auto* optFlowPtr = std::get_if<std::optional<muuid::uuid>>(&newProperties.flowId);
        if (optFlowPtr) clauses.append("flow_id = :flow_id");

        const auto* optTypePtr = std::get_if<std::optional<muuid::uuid>>(&newProperties.flowId);
        if (optTypePtr) clauses.append("type_id = :type_id");

        const auto* optStylePtr = std::get_if<std::optional<muuid::uuid>>(&newProperties.flowId);
        if (optStylePtr) clauses.append("style_id = :style_id");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE pin SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.id)              query.bindValue(":id",                    Utility::UUID::uuidToBytes(*newProperties.id));
        if (newProperties.contributorId)   query.bindValue(":new_contributor_id",    Utility::UUID::uuidToBytes(*newProperties.contributorId));
        if (newProperties.visualFactoryId) query.bindValue(":new_visual_factory_id", Utility::UUID::uuidToBytes(*newProperties.visualFactoryId));

        if (optFlowPtr)  query.bindValue(":flow_id",  optFlowPtr->has_value()  ? QVariant(Utility::UUID::uuidToBytes(optFlowPtr->value()))  : QVariant(QMetaType::fromType<QByteArray>()));
        if (optTypePtr)  query.bindValue(":type_id",  optTypePtr->has_value()  ? QVariant(Utility::UUID::uuidToBytes(optTypePtr->value()))  : QVariant(QMetaType::fromType<QByteArray>()));
        if (optStylePtr) query.bindValue(":style_id", optStylePtr->has_value() ? QVariant(Utility::UUID::uuidToBytes(optStylePtr->value())) : QVariant(QMetaType::fromType<QByteArray>()));

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update pin:" << query.lastError().text();
            return false;
        }
        return true;
    }
}