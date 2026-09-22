#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "Config.h"

namespace NDWireDetails::Create {

    inline bool createWireCore(QSqlQuery& query, const NDWireDetails::Config::CreateWireCoreRecord& newWireCore) {
        query.prepare(R"(
            INSERT OR IGNORE INTO wire_core (id,  contributor_id,  visual_factory_id,  style_id,  data_id,  name)
            VALUES (                        :id, :contributor_id, :visual_factory_id, :style_id, :data_id, :name);
        )");

        query.bindValue(":id",                Utility::UUID::uuidToBytes(newWireCore.id));
        query.bindValue(":contributor_id",    Utility::UUID::uuidToBytes(newWireCore.contributorId));
        query.bindValue(":visual_factory_id", Utility::UUID::uuidToBytes(newWireCore.visualFactoryId));
        query.bindValue(":style_id", newWireCore.styleId ? Utility::UUID::uuidToBytes(*newWireCore.styleId) : QVariant());
        query.bindValue(":data_id",  newWireCore.dataId  ? Utility::UUID::uuidToBytes(*newWireCore.dataId)  : QVariant());
        query.bindValue(":name",     newWireCore.name);

        if (!query.exec()) {
            qCritical() << "Failed to insert wire core:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createWireTemporary(QSqlQuery& query, const NDWireDetails::Config::CreateWireTemporaryRecord & newWireTemporary) {
        query.prepare(R"(
            INSERT OR IGNORE INTO wire_temporary (id,  core_id)
            VALUES (                             :id, :core_id);
        )");

        query.bindValue(":id",      Utility::UUID::uuidToBytes(newWireTemporary.id));
        query.bindValue(":core_id", Utility::UUID::uuidToBytes(newWireTemporary.coreId));

        if (!query.exec()) {
            qCritical() << "Failed to insert wire temporary:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createWire(QSqlQuery& query, const NDWireDetails::Config::CreateWireRecord& newWire) {
        query.prepare(R"(
            INSERT OR IGNORE INTO wire (id,  core_id,  origin_id,  origin_canvas_hint_x,  origin_canvas_hint_y,  target_id,  target_canvas_hint_x,  target_canvas_hint_y,  state)
            VALUES (                   :id, :core_id, :origin_id, :origin_canvas_hint_x, :origin_canvas_hint_y, :target_id, :target_canvas_hint_x, :target_canvas_hint_y, :state);
        )");

        query.bindValue(":id",      Utility::UUID::uuidToBytes(newWire.id));
        query.bindValue(":core_id", Utility::UUID::uuidToBytes(newWire.coreId));

        query.bindValue(":origin_id",            Utility::UUID::uuidToBytes(newWire.originId));
        query.bindValue(":origin_canvas_hint_x", newWire.originHintPos.x());
        query.bindValue(":origin_canvas_hint_y", newWire.originHintPos.y());

        query.bindValue(":target_id",            Utility::UUID::uuidToBytes(newWire.targetId));
        query.bindValue(":target_canvas_hint_x", newWire.targetHintPos.x());
        query.bindValue(":target_canvas_hint_y", newWire.targetHintPos.y());

        query.bindValue(":state",  newWire.state ? QVariant(Utility::ByteArray::toQByteArray(*newWire.state)) : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to insert wire:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createSymmetricWire(QSqlQuery& query, const NDWireDetails::Config::CreateSymmetricWireRecord& newSymmetricWire) {
        query.prepare(R"(
            INSERT OR IGNORE INTO symmetric_wire_registry (pin_type_id,  wire_core_id)
            VALUES (                                      :pin_type_id, :wire_core_id);
        )");

        query.bindValue(":pin_type_id",  Utility::UUID::uuidToBytes(newSymmetricWire.pinTypeId));
        query.bindValue(":wire_core_id", Utility::UUID::uuidToBytes(newSymmetricWire.wireCoreId));

        if (!query.exec()) {
            qCritical() << "Failed to insert symmetric wire:" << query.lastError().text();
            return false;
        }
        return true;
    }
    inline bool createAsymmetricWire(QSqlQuery& query, const NDWireDetails::Config::CreateAsymmetricWireRecord& newAsymmetricWire) {
        query.prepare(R"(
            INSERT OR IGNORE INTO asymmetric_wire_registry (origin_pin_type_id,  target_pin_type_id,  wire_core_id)
            VALUES (                                       :origin_pin_type_id, :target_pin_type_id, :wire_core_id);
        )");

        query.bindValue(":origin_pin_type_id", Utility::UUID::uuidToBytes(newAsymmetricWire.originPinTypeId));
        query.bindValue(":target_pin_type_id", Utility::UUID::uuidToBytes(newAsymmetricWire.targetPinTypeId));
        query.bindValue(":wire_core_id",       Utility::UUID::uuidToBytes(newAsymmetricWire.wireCoreId));

        if (!query.exec()) {
            qCritical() << "Failed to insert asymmetric wire:" << query.lastError().text();
            return false;
        }
        return true;
    }
}