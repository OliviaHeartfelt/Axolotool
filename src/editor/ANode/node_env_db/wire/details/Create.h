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
    inline bool createWirePins(QSqlQuery& query, const NDWireDetails::Config::CreateWirePinRecord<State>& newWirePins) {
        query.prepare(R"(
            INSERT INTO wire_pins (id,  core_id,  origin_id,  origin_canvas_hint_x,  origin_canvas_hint_y,  target_id,  target_canvas_hint_x,  target_canvas_hint_y,  state)
            VALUES (              :id, :core_id, :origin_id, :origin_canvas_hint_x, :origin_canvas_hint_y, :target_id, :target_canvas_hint_x, :target_canvas_hint_y, :state);
        )");

        query.bindValue(":id",      Utility::UUID::uuidToBytes(newWirePins.id));
        query.bindValue(":core_id", Utility::UUID::uuidToBytes(newWirePins.coreId));

        query.bindValue(":origin_id",            Utility::UUID::uuidToBytes(newWirePins.originId));
        query.bindValue(":origin_canvas_hint_x", newWirePins.originHintPos.x());
        query.bindValue(":origin_canvas_hint_y", newWirePins.originHintPos.y());

        query.bindValue(":target_id",            Utility::UUID::uuidToBytes(newWirePins.targetId));
        query.bindValue(":target_canvas_hint_x", newWirePins.targetHintPos.x());
        query.bindValue(":target_canvas_hint_y", newWirePins.targetHintPos.y());

        query.bindValue(":state",  newWirePins.state ? QVariant(newWirePins.state->classToByteArray()) : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to insert wire pins:" << query.lastError().text();
            return false;
        }
        return true;
    }
    template<NDConcepts::ByteConvertible State>
    inline bool createWireWidgets(QSqlQuery& query, const NDWireDetails::Config::CreateWireWidgetsRecord<State>& newWireWidgets) {
        query.prepare(R"(
            INSERT INTO wire_widgets (id,  core_id,  origin_id,  origin_canvas_hint_x,  origin_canvas_hint_y,  target_id,  target_canvas_hint_x,  target_canvas_hint_y,  state)
            VALUES (                 :id, :core_id, :origin_id, :origin_canvas_hint_x, :origin_canvas_hint_y, :target_id, :target_canvas_hint_x, :target_canvas_hint_y, :state);
        )");

        query.bindValue(":id",      Utility::UUID::uuidToBytes(newWireWidgets.id));
        query.bindValue(":core_id", Utility::UUID::uuidToBytes(newWireWidgets.coreId));

        query.bindValue(":origin_id",            Utility::UUID::uuidToBytes(newWireWidgets.originId));
        query.bindValue(":origin_canvas_hint_x", newWireWidgets.originHintPos.x());
        query.bindValue(":origin_canvas_hint_y", newWireWidgets.originHintPos.y());

        query.bindValue(":target_id",            Utility::UUID::uuidToBytes(newWireWidgets.targetId));
        query.bindValue(":target_canvas_hint_x", newWireWidgets.targetHintPos.x());
        query.bindValue(":target_canvas_hint_y", newWireWidgets.targetHintPos.y());

        query.bindValue(":state",  newWireWidgets.state ? QVariant(newWireWidgets.state->classToByteArray()) : QVariant());

        if (!query.exec()) {
            qCritical() << "Failed to insert wire widgets:" << query.lastError().text();
            return false;
        }
        return true;
    }
    template<NDConcepts::ByteConvertible State>
    inline bool createWireArbitrary(QSqlQuery& query, const NDWireDetails::Config::CreateWireArbitraryRecord<State>& newWireArbitrary) {
        query.prepare(R"(
            INSERT INTO wire_arbitrary (id,  core_id,  origin_pin_id,  origin_widget_id,  origin_canvas_hint_x,  origin_canvas_hint_y,  target_pin_id,  target_widget_id,  target_canvas_hint_x,  target_canvas_hint_y,  state)
            VALUES (                   :id, :core_id, :origin_pin_id, :origin_widget_id, :origin_canvas_hint_x, :origin_canvas_hint_y, :target_pin_id, :target_widget_id, :target_canvas_hint_x, :target_canvas_hint_y, :state);
        )");

        query.bindValue(":id",      Utility::UUID::uuidToBytes(newWireArbitrary.id));
        query.bindValue(":core_id", Utility::UUID::uuidToBytes(newWireArbitrary.coreId));

        // origin
        std::visit([&](const auto& v) {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, NDWireDetails::Config::PinVertex>) {
                query.bindValue(":origin_pin_id", Utility::UUID::uuidToBytes(v.id));
                query.bindValue(":origin_widget_id", QVariant());
            }
            else if constexpr (std::is_same_v<T, NDWireDetails::Config::WidgetVertex>) {
                query.bindValue(":origin_pin_id", QVariant());
                query.bindValue(":origin_widget_id", Utility::UUID::uuidToBytes(v.id));
            }
        }, newWireArbitrary.origin);

        query.bindValue(":origin_canvas_hint_x", newWireArbitrary.originHintPos.x());
        query.bindValue(":origin_canvas_hint_y", newWireArbitrary.originHintPos.y());

        // target
        std::visit([&](const auto& v) {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, NDWireDetails::Config::PinVertex>) {
                query.bindValue(":target_pin_id", Utility::UUID::uuidToBytes(v.id));
                query.bindValue(":target_widget_id", QVariant());
            }
            else if constexpr (std::is_same_v<T, NDWireDetails::Config::WidgetVertex>) {
                query.bindValue(":target_pin_id", QVariant());
                query.bindValue(":target_widget_id", Utility::UUID::uuidToBytes(v.id));
            }
        }, newWireArbitrary.target);

        query.bindValue(":target_canvas_hint_x", newWireArbitrary.targetHintPos.x());
        query.bindValue(":target_canvas_hint_y", newWireArbitrary.targetHintPos.y());


        if (newWireArbitrary.state)
            query.bindValue(":state", QVariant(newWireArbitrary.state->classToByteArray()));
        else
            query.bindValue(":state", QVariant(QMetaType::fromType<QByteArray>()));

        if (!query.exec()) {
            qCritical() << "Failed to insert wire arbitrary:" << query.lastError().text();
            return false;
        }
        return true;
    }
}