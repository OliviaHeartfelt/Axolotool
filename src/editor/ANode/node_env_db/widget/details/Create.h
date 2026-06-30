#pragma once

#include "../../../../Utility/Utility.h"
#include "Config.h"

namespace NDWidgetDetails::Create {

    inline std::optional<muuid::uuid> createWidgetCore(QSqlQuery& query, const NDWidgetDetails::Config::CreateWidgetCoreRecord& newWidgetCore) {
        muuid::uuid coreId = muuid::uuid::generate_unix_time_based();
        query.prepare(R"(
            INSERT INTO widget_core (id, contributor_id, type_id, data_id)
            VALUES (:id, :contributor_id, :flow, :type_id, :data_id);
        )");

        query.bindValue(":id",             Utility::UUID::uuidToBytes(coreId));
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(newWidgetCore.contributorId));
        query.bindValue(":type_id",        newWidgetCore.typeId ? Utility::UUID::uuidToBytes(*newWidgetCore.typeId) : QVariant());
        query.bindValue(":data_id",        newWidgetCore.dataId ? Utility::UUID::uuidToBytes(*newWidgetCore.dataId) : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to insert pin:" << query.lastError().text();
            return std::nullopt;
        }
        return coreId;
    }
    template<NDWidgetDetails::Config::WidgetState State>
    inline std::optional<muuid::uuid> createWidget(QSqlQuery& query, const NDWidgetDetails::Config::CreateWidgetRecord& newWidget, const std::optional<State>& state = std::nullopt) {
        muuid::uuid widgetId = muuid::uuid::generate_unix_time_based();
        query.prepare(R"(
            INSERT INTO widget (id, core_id, state, w_size, h_size)
            VALUES (:id, :core_id, :state, :w_size, :h_size);
        )");

        query.bindValue(":id",      Utility::UUID::uuidToBytes(widgetId));
        query.bindValue(":core_id", Utility::UUID::uuidToBytes(newWidget.coreId));
        query.bindValue(":state",   state ? QVariant(state->classToByteArray()) : QVariant());
        query.bindValue(":w_size",  newWidget.w ? *newWidget.w : QVariant());
        query.bindValue(":h_size",  newWidget.h ? *newWidget.h : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to insert pin:" << query.lastError().text();
            return std::nullopt;
        }
        return widgetId;
    }
}