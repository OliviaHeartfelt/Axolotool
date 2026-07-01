#pragma once

#include "../../../../Utility/Utility.h"
#include "Config.h"

namespace NDWidgetDetails::Update {

    // 1. Widget Core
    inline bool updateWidgetCore(QSqlQuery& query, muuid::uuid id, const NDWidgetDetails::Config::updateWidgetCoreRecord& newProperties) {
        QStringList clauses;
    
        if (std::holds_alternative<std::optional<muuid::uuid>>(newProperties.contributorId)) clauses.append("contributor_id = :contributor_id");
        if (std::holds_alternative<std::optional<muuid::uuid>>(newProperties.typeId))        clauses.append("type_id = :type_id");
        if (std::holds_alternative<std::optional<muuid::uuid>>(newProperties.dataId))        clauses.append("data_id = :data_id");
    
        if (clauses.isEmpty()) return true;
    
        QString updateSql = QString("UPDATE widget_core SET %1 WHERE id = :id;").arg(clauses.join(", "));
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
    
        if (std::holds_alternative<std::optional<muuid::uuid>>(newProperties.contributorId)) {
            query.bindValue(":contributor_id", unpackUpdateField(newProperties.contributorId));
        }
        if (std::holds_alternative<std::optional<muuid::uuid>>(newProperties.typeId)) {
            query.bindValue(":type_id", unpackUpdateField(newProperties.typeId));
        }
        if (std::holds_alternative<std::optional<muuid::uuid>>(newProperties.dataId)) {
            query.bindValue(":data_id", unpackUpdateField(newProperties.dataId));
        }
    
        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update widget core:" << query.lastError().text();
            return false;
        }
        return true;
    }
    
    // 2. Widget
    template<NDWidgetDetails::Config::ByteConvertible State>
    inline bool updateWidget(QSqlQuery& query, muuid::uuid id, const NDWidgetDetails::Config::UpdateWidgetRecord<State>& newProperties) {
        QStringList clauses;
    
        if (std::holds_alternative<std::optional<State>>(newProperties.state)) clauses.append("state = :state");
        if (std::holds_alternative<std::optional<qreal>>(newProperties.w))     clauses.append("w_size = :w_size");
        if (std::holds_alternative<std::optional<qreal>>(newProperties.h))     clauses.append("h_size = :h_size");
    
        if (clauses.isEmpty()) return true;
    
        QString updateSql = QString("UPDATE widget SET %1 WHERE id = :id;").arg(clauses.join(", "));
        if (!query.prepare(updateSql)) {
            qCritical() << "Failed to prepare dynamic update query:" << query.lastError().text();
            return false;
        }
    
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));
    
        if (std::holds_alternative<std::optional<State>>(newProperties.state)) {
            if (const auto* opt = std::get_if<std::optional<State>>(&newProperties.state)) {
                query.bindValue(":state", *opt ? (*opt)->classToByteArray() : QVariant(QVariant::fromValue(nullptr)));
            }
        }
        if (std::holds_alternative<std::optional<qreal>>(newProperties.w)) {
            if (const auto* opt = std::get_if<std::optional<qreal>>(&newProperties.w)) {
                query.bindValue(":w_size", *opt ? QVariant(**opt) : QVariant(QVariant::fromValue(nullptr)));
            }
        }
        if (std::holds_alternative<std::optional<qreal>>(newProperties.h)) {
            if (const auto* opt = std::get_if<std::optional<qreal>>(&newProperties.h)) {
                query.bindValue(":h_size", *opt ? QVariant(**opt) : QVariant(QVariant::fromValue(nullptr)));
            }
        }
    
        if (!query.exec()) {
            qCritical() << "Failed to execute dynamic update widget:" << query.lastError().text();
            return false;
        }
        return true;
    }
}