#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "Config.h"

namespace NDWireDetails::Create {

    inline bool createWireCore(QSqlQuery& query, const NDWireDetails::Config::CreateWireCoreRecord& newWireCore) {
        query.prepare(R"(
            INSERT INTO widget_core (id,  contributor_id,  style_id,  data_id,  name)
            VALUES (                :id, :contributor_id, :style_id, :data_id, :name);
        )");

        query.bindValue(":id",             Utility::UUID::uuidToBytes(newWireCore.id));
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(newWireCore.contributorId));
        query.bindValue(":style_id",       newWireCore.styleId ? Utility::UUID::uuidToBytes(*newWireCore.styleId) : QVariant());
        query.bindValue(":data_id",        newWireCore.dataId  ? Utility::UUID::uuidToBytes(*newWireCore.dataId)  : QVariant());
        query.bindValue(":name",           newWireCore.name);

        if (!query.exec()) {
            qCritical() << "Failed to insert wire core:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createWireTemporary(QSqlQuery& query, const NDWireDetails::Config::CreateWireTemporaryRecord & newWireTemporary) {
        query.prepare(R"(
            INSERT INTO wire_temporary (id,  core_id)
            VALUES (                   :id, :core_id);
        )");

        query.bindValue(":id",      Utility::UUID::uuidToBytes(newWireTemporary.id));
        query.bindValue(":core_id", Utility::UUID::uuidToBytes(newWireTemporary.coreId));

        if (!query.exec()) {
            qCritical() << "Failed to insert wire temporary:" << query.lastError().text();
            return false;
        }
        return true;
    }

    template<NDConcepts::ByteConvertible State>
    inline bool createWire(QSqlQuery& query, const NDWireDetails::Config::CreateWireRecord<State>& newWirePins) {
        query.prepare(R"(
            INSERT INTO wire (id,  core_id,  origin_id,  origin_canvas_hint_x,  origin_canvas_hint_y,  target_id,  target_canvas_hint_x,  target_canvas_hint_y,  state)
            VALUES (         :id, :core_id, :origin_id, :origin_canvas_hint_x, :origin_canvas_hint_y, :target_id, :target_canvas_hint_x, :target_canvas_hint_y, :state);
        )");

        query.bindValue(":id",      Utility::UUID::uuidToBytes(newWirePins.id));
        query.bindValue(":core_id", Utility::UUID::uuidToBytes(newWirePins.coreId));

        query.bindValue(":origin_id",            Utility::UUID::uuidToBytes(newWirePins.originId));
        query.bindValue(":origin_canvas_hint_x", newWirePins.originHintPos.x());
        query.bindValue(":origin_canvas_hint_y", newWirePins.originHintPos.y());

        query.bindValue(":target_id",            Utility::UUID::uuidToBytes(newWirePins.targetId));
        query.bindValue(":target_canvas_hint_x", newWirePins.targetHintPos.x());
        query.bindValue(":target_canvas_hint_y", newWirePins.targetHintPos.y());

        query.bindValue(":state",  newWirePins.state ? QVariant(Utility::ByteArray::toQByteArray(newWirePins.state->classToBytes())) : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to insert wire:" << query.lastError().text();
            return false;
        }
        return true;
    }
}