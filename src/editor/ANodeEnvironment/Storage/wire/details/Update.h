#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "../../NDHelpers.h"
#include "Config.h"

namespace NDWireDetails::Update {

    inline bool updateWireCore(QSqlQuery& query, muuid::uuid id, const NDWireDetails::Config::UpdateWireCoreRecord& newProperties) {
        QStringList clauses;

        if (newProperties.id)            clauses.append("id = :new_id");
        if (newProperties.contributorId) clauses.append("contributor_id = :new_contributor_id");

        if (newProperties.styleId) clauses.append("style_id = :style_id");
        if (newProperties.dataId)  clauses.append("data_id = :data_id");
        if (newProperties.name)    clauses.append("name = :name");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE wire_core SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query for wire_core:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.id)            query.bindValue(":new_id",             Utility::UUID::uuidToBytes(*newProperties.id));
        if (newProperties.contributorId) query.bindValue(":new_contributor_id", Utility::UUID::uuidToBytes(*newProperties.contributorId));

        if (newProperties.styleId) query.bindValue(":style_id", Utility::UUID::uuidToBytes(*newProperties.styleId));
        if (newProperties.dataId)  query.bindValue(":data_id",  Utility::UUID::uuidToBytes(*newProperties.dataId));
        if (newProperties.name)    query.bindValue(":name",     *newProperties.name);

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update wire core:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool updateWire(QSqlQuery& query, muuid::uuid id, const NDWireDetails::Config::UpdateWireRecord& newProperties) {
        QStringList clauses;

        if (newProperties.id)     clauses.append("id = :new_id");
        if (newProperties.coreId) clauses.append("core_id = :new_core_id");

        if (newProperties.originHintPos) {
            clauses.append("origin_hint_pos_x = :origin_hint_pos_x");
            clauses.append("origin_hint_pos_y = :origin_hint_pos_y");
        }
        if (newProperties.targetHintPos) {
            clauses.append("target_hint_pos_x = :target_hint_pos_x");
            clauses.append("target_hint_pos_y = :target_hint_pos_y");
        }

        const auto* optPtr = std::get_if<std::optional<std::vector<uint8_t>>>(&newProperties.state);
        if (optPtr) clauses.append("state = :state");

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE wire SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query for wire:" << query.lastError().text();
            return false;
        }
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.id)     query.bindValue(":new_id",      Utility::UUID::uuidToBytes(*newProperties.id));
        if (newProperties.coreId) query.bindValue(":new_core_id", Utility::UUID::uuidToBytes(*newProperties.coreId));

        if (newProperties.originHintPos) {
            query.bindValue(":origin_hint_pos_x", newProperties.originHintPos->x());
            query.bindValue(":origin_hint_pos_y", newProperties.originHintPos->y());
        }
        if (newProperties.targetHintPos) {
            query.bindValue(":target_hint_pos_x", newProperties.targetHintPos->x());
            query.bindValue(":target_hint_pos_y", newProperties.targetHintPos->y());
        }

        if (optPtr) query.bindValue(":state", optPtr->has_value() ? QVariant(Utility::ByteArray::toQByteArray(optPtr->value())) : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update wire:" << query.lastError().text();
            return false;
        }
        return true;
    }
}