#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "Config.h"

namespace NDWidgetDetails::Update {

    // 1. Widget Core
    inline bool updateWidgetCore(QSqlQuery& query, const muuid::uuid& id, const NDWidgetDetails::Config::UpdateWidgetCoreRecord& newProperties) {
        QStringList clauses;

        if (newProperties.id)              clauses.append("id = :new_id");
        if (newProperties.contributorId)   clauses.append("contributor_id = :new_contributor_id");
        if (newProperties.visualFactoryId) clauses.append("visual_factory_id = :new_visual_factory_id");

        const auto* optTypePtr = std::get_if<std::optional<muuid::uuid>>(&newProperties.typeId);
        if (optTypePtr) clauses.append("type_id = :type_id");
    
        const auto* optDataPtr = std::get_if<std::optional<muuid::uuid>>(&newProperties.dataId);
        if (optDataPtr) clauses.append("data_id = :data_id");
    
        if (clauses.isEmpty()) return true;
    
        QString updateSql = QString("UPDATE widget_core SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.id)              query.bindValue(":new_id",                Utility::UUID::uuidToBytes(*newProperties.id));
        if (newProperties.contributorId)   query.bindValue(":contributor_id",        Utility::UUID::uuidToBytes(*newProperties.contributorId));
        if (newProperties.visualFactoryId) query.bindValue(":new_visual_factory_id", Utility::UUID::uuidToBytes(*newProperties.visualFactoryId));

        if (optTypePtr) query.bindValue(":type_id", optTypePtr->has_value() ? Utility::UUID::uuidToBytes(optTypePtr->value()) : QVariant(QMetaType::fromType<QByteArray>()));
        if (optDataPtr) query.bindValue(":type_id", optDataPtr->has_value() ? Utility::UUID::uuidToBytes(optDataPtr->value()) : QVariant(QMetaType::fromType<QByteArray>()));
    
        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update widget core:" << query.lastError().text();
            return false;
        }
        return true;
    }
    
    // 2. Widget
    inline bool updateWidget(QSqlQuery& query, const muuid::uuid& id, const NDWidgetDetails::Config::UpdateWidgetRecord& newProperties) {
        QStringList clauses;

        if (newProperties.id)     clauses.append("id = :new_id");
        if (newProperties.coreId) clauses.append("core_id = :new_core_id");
    
        const auto* optPtr = std::get_if<std::optional<std::vector<uint8_t>>>(&newProperties.state);
        if (optPtr) clauses.append("state = :state");

        if (newProperties.width)  clauses.append("w_size = :w_size");
        if (newProperties.height) clauses.append("h_size = :h_size");
    
        if (clauses.isEmpty()) return true;
    
        QString updateSql = QString("UPDATE widget SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.id)     query.bindValue(":new_id",      Utility::UUID::uuidToBytes(*newProperties.id));
        if (newProperties.coreId) query.bindValue(":new_core_id", Utility::UUID::uuidToBytes(*newProperties.coreId));

        if (optPtr) query.bindValue(":state", optPtr->has_value() ? QVariant(Utility::ByteArray::toQByteArray(optPtr->value())) : QVariant());

        if (newProperties.width)  query.bindValue(":w_size", QVariant(*newProperties.width));
        if (newProperties.height) query.bindValue(":h_size", QVariant(*newProperties.height));
    
        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update widget:" << query.lastError().text();
            return false;
        }
        return true;
    }
}