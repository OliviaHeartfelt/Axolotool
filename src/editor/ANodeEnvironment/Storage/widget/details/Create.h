#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "Config.h"

namespace NDWidgetDetails::Create {

    inline bool createWidgetCore(QSqlQuery& query, const NDWidgetDetails::Config::CreateWidgetCoreRecord& newWidgetCore) {
        query.prepare(R"(
            INSERT INTO widget_core (id,  contributor_id,  type_id,  data_id)
            VALUES (                :id, :contributor_id, :type_id, :data_id);
        )");

        query.bindValue(":id",             Utility::UUID::uuidToBytes(newWidgetCore.id));
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(newWidgetCore.contributorId));
        query.bindValue(":type_id",        newWidgetCore.typeId ? Utility::UUID::uuidToBytes(*newWidgetCore.typeId) : QVariant());
        query.bindValue(":data_id",        newWidgetCore.dataId ? Utility::UUID::uuidToBytes(*newWidgetCore.dataId) : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to insert widget core:" << query.lastError().text();
            return false;
        }
        return true;
    }
    template<NDConcepts::ByteConvertible State>
    inline bool createWidget(QSqlQuery& query, const NDWidgetDetails::Config::CreateWidgetRecord<State>& newWidget) {
        query.prepare(R"(
            INSERT INTO widget (id,  core_id,  state,  w_size,  h_size)
            VALUES (           :id, :core_id, :state, :w_size, :h_size);
        )");

        query.bindValue(":id",      Utility::UUID::uuidToBytes(newWidget.id));
        query.bindValue(":core_id", Utility::UUID::uuidToBytes(newWidget.coreId));
        query.bindValue(":state",   newWidget.state ? QVariant(Utility::ByteArray::toQByteArray(newWidget.state->classToBytes())) : QVariant());
        query.bindValue(":w_size",  newWidget.w ? *newWidget.w : QVariant());
        query.bindValue(":h_size",  newWidget.h ? *newWidget.h : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to insert widget:" << query.lastError().text();
            return false;
        }
        return true;
    }
}