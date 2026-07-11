#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "../../NDHelpers.h"
#include "Config.h"

namespace NDWireDetails::Update {

    inline bool updateWireCore(QSqlQuery& query, muuid::uuid id, const NDWireDetails::Config::UpdateWireCoreRecord& newProperties) {
        QStringList clauses;

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

        if (newProperties.styleId) query.bindValue(":style_id", Utility::UUID::uuidToBytes(*newProperties.styleId));
        if (newProperties.dataId)  query.bindValue(":data_id", Utility::UUID::uuidToBytes(*newProperties.dataId));
        if (newProperties.name)    query.bindValue(":name", *newProperties.name);

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update wire_core:" << query.lastError().text();
            return false;
        }
        return true;
    }
    template<NDConcepts::ByteConvertible State>
    inline bool updateWirePins(QSqlQuery& query, muuid::uuid id, const NDWireDetails::Config::UpdateWirePinRecord<State>& newProperties) {
        QStringList clauses;

        if (newProperties.originHintPos) {
            clauses.append("origin_hint_pos_x = :origin_hint_pos_x");
            clauses.append("origin_hint_pos_y = :origin_hint_pos_y");
        }
        if (newProperties.targetHintPos) {
            clauses.append("target_hint_pos_x = :target_hint_pos_x");
            clauses.append("target_hint_pos_y = :target_hint_pos_y");
        }

        const auto* optPtr = std::get_if<std::optional<State>>(&newProperties.state);
        if (optPtr) {
            clauses.append("state = :state");
        }

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE wire_pins SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query for wire_pins:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.originHintPos) {
            query.bindValue(":origin_hint_pos_x", newProperties.originHintPos->x());
            query.bindValue(":origin_hint_pos_y", newProperties.originHintPos->y());
        }
        if (newProperties.targetHintPos) {
            query.bindValue(":target_hint_pos_x", newProperties.targetHintPos->x());
            query.bindValue(":target_hint_pos_y", newProperties.targetHintPos->y());
        }

        if (optPtr) {
            if (optPtr->has_value())
                query.bindValue(":state", QVariant(optPtr->value().classToByteArray()));
            else
                query.bindValue(":state", QVariant(QMetaType::fromType<QByteArray>()));
        }

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update wire_pins:" << query.lastError().text();
            return false;
        }
        return true;
    }
    template<NDConcepts::ByteConvertible State>
    inline bool updateWireWidgets(QSqlQuery& query, muuid::uuid id, const NDWireDetails::Config::UpdateWireWidgetsRecord<State>& newProperties) {
        QStringList clauses;

        if (newProperties.originHintPos) {
            clauses.append("origin_hint_pos_x = :origin_hint_pos_x");
            clauses.append("origin_hint_pos_y = :origin_hint_pos_y");
        }
        if (newProperties.targetHintPos) {
            clauses.append("target_hint_pos_x = :target_hint_pos_x");
            clauses.append("target_hint_pos_y = :target_hint_pos_y");
        }
        const auto* optPtr = std::get_if<std::optional<State>>(&newProperties.state);
        if (optPtr) {
            clauses.append("state = :state");
        }

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE wire_widgets SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query for wire_widgets:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.originHintPos) {
            query.bindValue(":origin_hint_pos_x", newProperties.originHintPos->x());
            query.bindValue(":origin_hint_pos_y", newProperties.originHintPos->y());
        }
        if (newProperties.targetHintPos) {
            query.bindValue(":target_hint_pos_x", newProperties.targetHintPos->x());
            query.bindValue(":target_hint_pos_y", newProperties.targetHintPos->y());
        }

        if (optPtr) {
            if (optPtr->has_value())
                query.bindValue(":state", QVariant(optPtr->value().classToByteArray()));
            else
                query.bindValue(":state", QVariant(QMetaType::fromType<QByteArray>()));
        }

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update wire_widgets:" << query.lastError().text();
            return false;
        }
        return true;
    }
    template<NDConcepts::ByteConvertible State>
    inline bool updateWireArbitrary(QSqlQuery& query, muuid::uuid id, const NDWireDetails::Config::UpdateWireArbitraryRecord<State>& newProperties) {
        QStringList clauses;

        if (newProperties.originHintPos) {
            clauses.append("origin_hint_pos_x = :origin_hint_pos_x");
            clauses.append("origin_hint_pos_y = :origin_hint_pos_y");
        }
        if (newProperties.targetHintPos) {
            clauses.append("target_hint_pos_x = :target_hint_pos_x");
            clauses.append("target_hint_pos_y = :target_hint_pos_y");
        }
        const auto* optPtr = std::get_if<std::optional<State>>(&newProperties.state);
        if (optPtr) {
            clauses.append("state = :state");
        }

        if (clauses.isEmpty()) return true;

        QString updateSql = QString("UPDATE wire_arbitrary SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query for wire_arbitrary:" << query.lastError().text();
            return false;
        }

        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (newProperties.originHintPos) {
            query.bindValue(":origin_hint_pos_x", newProperties.originHintPos->x());
            query.bindValue(":origin_hint_pos_y", newProperties.originHintPos->y());
        }
        if (newProperties.targetHintPos) {
            query.bindValue(":target_hint_pos_x", newProperties.targetHintPos->x());
            query.bindValue(":target_hint_pos_y", newProperties.targetHintPos->y());
        }

        if (optPtr) {
            if (optPtr->has_value())
                query.bindValue(":state", QVariant(optPtr->value().classToByteArray()));
            else
                query.bindValue(":state", QVariant(QMetaType::fromType<QByteArray>()));
        }

        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update wire_arbitrary:" << query.lastError().text();
            return false;
        }
        return true;
    }
}