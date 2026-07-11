#pragma once

#include "../../../../Utility/Utility.h"
#include "../../NDConcepts.h"
#include "../../NDHelpers.h"
#include "Config.h"

namespace NDWireDetails::Read {

    // 1. Wire Core
    inline std::optional<NDWireDetails::Config::FullWireCoreRecord> getWireCore(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT contributor_id, style_id, data_id, name
            FROM wire_core 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getWireCore query:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const auto contributorId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!contributorId) return std::nullopt;

        const NDHelpers::NullableField<muuid::uuid> styleId = NDHelpers::parseNullableUUID(query.value(1));
        if (styleId.isCorrupted()) return std::nullopt;

        const NDHelpers::NullableField<muuid::uuid> dataId = NDHelpers::parseNullableUUID(query.value(2));
        if (dataId.isCorrupted()) return std::nullopt;

        return NDWireDetails::Config::FullWireCoreRecord{
            id,
            *contributorId,
            styleId.value,
            dataId.value,
            query.value(3).toString()
        };
    }
    inline std::optional<QList<NDWireDetails::Config::FullWireCoreRecord>> getContributorWireCores(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
        QList<NDWireDetails::Config::FullWireCoreRecord> list;

        query.prepare(R"(
            SELECT id, style_id, data_id, name
            FROM wire_core 
            WHERE contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getContributorWireCores query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());

            const NDHelpers::NullableField<muuid::uuid> styleId = NDHelpers::parseNullableUUID(query.value(1));
            const NDHelpers::NullableField<muuid::uuid> dataId = NDHelpers::parseNullableUUID(query.value(2));

            if (!id || styleId.isCorrupted() || dataId.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireDetails::Config::FullWireCoreRecord{
                *id,
                contributorId,
                styleId.value,
                dataId.value,
                query.value(3).toString()
            });
        }
        return list;
    }
    inline std::optional<QList<NDWireDetails::Config::FullWireCoreRecord>> getAllWireCores(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDWireDetails::Config::FullWireCoreRecord> list;

        query.prepare(R"(
            SELECT wc.id, wc.contributor_id, wc.style_id, wc.data_id, wc.name
            FROM wire_core wc
            INNER JOIN widget_contributor c ON wc.contributor_id = c.id
            WHERE c.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllWireCores query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto contributorId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());

            const NDHelpers::NullableField<muuid::uuid> styleId = NDHelpers::parseNullableUUID(query.value(2));
            const NDHelpers::NullableField<muuid::uuid> dataId = NDHelpers::parseNullableUUID(query.value(3));

            if (!id || !contributorId || styleId.isCorrupted() || dataId.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireDetails::Config::FullWireCoreRecord{
                *id,
                *contributorId,
                styleId.value,
                dataId.value,
                query.value(4).toString()
            });
        }
        return list;
    }

    // 2. Wire Temporary
    inline std::optional<NDWireDetails::Config::FullWireTemporaryRecord> getWireTemporary(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT core_id
            FROM wire_temporary 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getWireTemporary query:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const auto coreId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!coreId) return std::nullopt;

        return NDWireDetails::Config::FullWireTemporaryRecord{
            id,
            *coreId
        };
    }
    inline std::optional<QList<NDWireDetails::Config::FullWireTemporaryRecord>> getContributorWireTemporary(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
        QList<NDWireDetails::Config::FullWireTemporaryRecord> list;

        query.prepare(R"(
            SELECT wt.id, wt.core_id
            FROM wire_temporary wt
            INNER JOIN wire_core cr ON wt.core_id = cr.id
            WHERE cr.contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getContributorWireTemporary query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto coreId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());

            if (!id || !coreId) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireDetails::Config::FullWireTemporaryRecord{
                *id,
                *coreId,
            });
        }
        return list;
    }
    inline std::optional<QList<NDWireDetails::Config::FullWireTemporaryRecord>> getAllWireTemporary(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDWireDetails::Config::FullWireTemporaryRecord> list;

        query.prepare(R"(
            SELECT wt.id, wt.core_id
            FROM wire_temporary wt
            INNER JOIN wire_core cr ON wt.core_id = cr.id
            INNER JOIN widget_contributor c ON cr.contributor_id = c.id
            WHERE c.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllWireTemporary query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto coreId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());

            if (!id || !coreId) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireDetails::Config::FullWireTemporaryRecord{
                *id,
                *coreId
            });
        }
        return list;
    }

    // 3. Wire Pins
    template<NDConcepts::ByteConvertible State>
    inline std::optional<NDWireDetails::Config::FullWirePinRecord<State>> getWirePins(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT core_id, origin_id, target_id, origin_canvas_hint_x, origin_canvas_hint_y, target_canvas_hint_x, target_canvas_hint_y, state
            FROM wire_pins 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getWirePin query:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const auto coreId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!coreId) return std::nullopt;

        const auto originId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());
        if (!originId) return std::nullopt;

        const auto targetId = Utility::UUID::bytesToUuid(query.value(2).toByteArray());
        if (!targetId) return std::nullopt;

        const NDHelpers::NullableField<State> state = NDHelpers::parseNullableByteConvertible<State>(query.value(7));
        if (state.isCorrupted()) return std::nullopt;

        return NDWireDetails::Config::FullWirePinRecord<State>{
            id,
            *coreId,
            *originId,
            *targetId,
            QPointF(query.value(3).toReal(), query.value(4).toReal()),
            QPointF(query.value(5).toReal(), query.value(6).toReal()),
            state.value
        };
    }

    template<NDConcepts::ByteConvertible State>
    inline std::optional<QList<NDWireDetails::Config::FullWirePinRecord<State>>> getContributorWirePins(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
        QList<NDWireDetails::Config::FullWirePinRecord<State>> list;

        query.prepare(R"(
            SELECT wp.id, wp.core_id, wp.origin_id, wp.target_id, wp.origin_canvas_hint_x, wp.origin_canvas_hint_y, wp.target_canvas_hint_x, wp.target_canvas_hint_y, wp.state
            FROM wire_pins wp
            INNER JOIN wire_core cr ON wp.core_id = cr.id
            WHERE cr.contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getContributorWirePins query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto coreId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());
            const auto originId = Utility::UUID::bytesToUuid(query.value(2).toByteArray());
            const auto targetId = Utility::UUID::bytesToUuid(query.value(3).toByteArray());

            const NDHelpers::NullableField<State> state = NDHelpers::parseNullableByteConvertible<State>(query.value(8));

            if (!id || !coreId || !originId || !targetId || state.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireDetails::Config::FullWirePinRecord<State>{
                *id,
                *coreId,
                *originId,
                *targetId,
                QPointF(query.value(4).toReal(), query.value(5).toReal()),
                QPointF(query.value(6).toReal(), query.value(7).toReal()),
                state.value
            });
        }
        return list;
    }

    template<NDConcepts::ByteConvertible State>
    inline std::optional<QList<NDWireDetails::Config::FullWirePinRecord<State>>> getAllWirePins(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDWireDetails::Config::FullWirePinRecord<State>> list;

        query.prepare(R"(
            SELECT wp.id, wp.core_id, wp.origin_id, wp.target_id, wp.origin_canvas_hint_x, wp.origin_canvas_hint_y, wp.target_canvas_hint_x, wp.target_canvas_hint_y, wp.state
            FROM wire_pins wp
            INNER JOIN wire_core cr ON wp.core_id = cr.id
            INNER JOIN widget_contributor c ON cr.contributor_id = c.id
            WHERE c.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllWirePins query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto coreId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());
            const auto originId = Utility::UUID::bytesToUuid(query.value(2).toByteArray());
            const auto targetId = Utility::UUID::bytesToUuid(query.value(3).toByteArray());

            const NDHelpers::NullableField<State> state = NDHelpers::parseNullableByteConvertible<State>(query.value(8));

            if (!id || !coreId || !originId || !targetId || state.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireDetails::Config::FullWirePinRecord<State>{
                *id,
                *coreId,
                *originId,
                *targetId,
                QPointF(query.value(4).toReal(), query.value(5).toReal()),
                QPointF(query.value(6).toReal(), query.value(7).toReal()),
                state.value
            });
        }
        return list;
    }

    // 4. Wire Widgets
    template<NDConcepts::ByteConvertible State>
    inline std::optional<NDWireDetails::Config::FullWireWidgetsRecord<State>> getWireWidgets(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT core_id, origin_id, target_id, origin_canvas_hint_x, origin_canvas_hint_y, target_canvas_hint_x, target_canvas_hint_y, state
            FROM wire_widgets 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getWireWidget query:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        const auto coreId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!coreId) return std::nullopt;

        const auto originId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());
        if (!originId) return std::nullopt;

        const auto targetId = Utility::UUID::bytesToUuid(query.value(2).toByteArray());
        if (!targetId) return std::nullopt;

        const NDHelpers::NullableField<State> state = NDHelpers::parseNullableByteConvertible<State>(query.value(7));
        if (state.isCorrupted()) return std::nullopt;

        return NDWireDetails::Config::FullWireWidgetsRecord<State>{
            id,
            *coreId,
            *originId,
            *targetId,
            QPointF(query.value(3).toReal(), query.value(4).toReal()),
            QPointF(query.value(5).toReal(), query.value(6).toReal()),
            state.value
        };
    }

    template<NDConcepts::ByteConvertible State>
    inline std::optional<QList<NDWireDetails::Config::FullWireWidgetsRecord<State>>> getContributorWireWidgets(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
        QList<NDWireDetails::Config::FullWireWidgetsRecord<State>> list;

        query.prepare(R"(
            SELECT ww.id, ww.core_id, ww.origin_id, ww.target_id, ww.origin_canvas_hint_x, ww.origin_canvas_hint_y, ww.target_canvas_hint_x, ww.target_canvas_hint_y, ww.state
            FROM wire_widgets ww
            INNER JOIN wire_core cr ON ww.core_id = cr.id
            WHERE cr.contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getContributorWireWidgets query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto coreId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());
            const auto originId = Utility::UUID::bytesToUuid(query.value(2).toByteArray());
            const auto targetId = Utility::UUID::bytesToUuid(query.value(3).toByteArray());

            const NDHelpers::NullableField<State> state = NDHelpers::parseNullableByteConvertible<State>(query.value(8));

            if (!id || !coreId || !originId || !targetId || state.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireDetails::Config::FullWireWidgetsRecord<State>{
                *id,
                *coreId,
                *originId,
                *targetId,
                QPointF(query.value(4).toReal(), query.value(5).toReal()),
                QPointF(query.value(6).toReal(), query.value(7).toReal()),
                state.value
            });
        }
        return list;
    }

    template<NDConcepts::ByteConvertible State>
    inline std::optional<QList<NDWireDetails::Config::FullWireWidgetsRecord<State>>> getAllWireWidgets(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDWireDetails::Config::FullWireWidgetsRecord<State>> list;

        query.prepare(R"(
            SELECT ww.id, ww.core_id, ww.origin_id, ww.target_id, ww.origin_canvas_hint_x, ww.origin_canvas_hint_y, ww.target_canvas_hint_x, ww.target_canvas_hint_y, ww.state
            FROM wire_widgets ww
            INNER JOIN wire_core cr ON ww.core_id = cr.id
            INNER JOIN widget_contributor c ON cr.contributor_id = c.id
            WHERE c.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllWireWidgets query:" << query.lastError().text();
            return std::nullopt;
        }

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto coreId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());
            const auto originId = Utility::UUID::bytesToUuid(query.value(2).toByteArray());
            const auto targetId = Utility::UUID::bytesToUuid(query.value(3).toByteArray());

            const NDHelpers::NullableField<State> state = NDHelpers::parseNullableByteConvertible<State>(query.value(8));

            if (!id || !coreId || !originId || !targetId || state.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireDetails::Config::FullWireWidgetsRecord<State>{
                *id,
                *coreId,
                *originId,
                *targetId,
                QPointF(query.value(4).toReal(), query.value(5).toReal()),
                QPointF(query.value(6).toReal(), query.value(7).toReal()),
                state.value
            });
        }
        return list;
    }

    // 5. Wire Arbitrary
    template<NDConcepts::ByteConvertible State>
    inline std::optional<NDWireDetails::Config::FullWireArbitraryRecord<State>> getWireArbitrary(QSqlQuery& query, const muuid::uuid& id) {
        query.prepare(R"(
            SELECT core_id, origin_pin_id, origin_widget_id, origin_canvas_hint_x, origin_canvas_hint_y, target_pin_id, target_widget_id, target_canvas_hint_x, target_canvas_hint_y, state                       
            FROM wire_arbitrary 
            WHERE id = :id;
        )");
        query.bindValue(":id", Utility::UUID::uuidToBytes(id));

        if (!query.exec()) {
            qCritical() << "Failed to execute getWireArbitrary query:" << query.lastError().text();
            return std::nullopt;
        }
        if (!query.next()) return std::nullopt;

        auto makeVertex = [](const QVariant& pin, const QVariant& widget) -> std::optional<NDWireDetails::Config::Vertex> {
            if (pin.isNull() == widget.isNull())
                return std::nullopt;

            if (!pin.isNull()) {
                const auto id = Utility::UUID::bytesToUuid(pin.toByteArray());
                if (!id) return std::nullopt;
                return NDWireDetails::Config::PinVertex{ *id };
            }
            else if (!widget.isNull()) {
                const auto id = Utility::UUID::bytesToUuid(widget.toByteArray());
                if (!id) return std::nullopt;
                return NDWireDetails::Config::WidgetVertex{ *id };
            }
            return std::nullopt;
            };

        const auto coreId = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
        if (!coreId) return std::nullopt;

        const std::optional<NDWireDetails::Config::Vertex> origin = makeVertex(query.value(1), query.value(2));
        if (!origin) return std::nullopt;

        const std::optional<NDWireDetails::Config::Vertex> target = makeVertex(query.value(5), query.value(6));
        if (!target) return std::nullopt;

        const NDHelpers::NullableField<State> state = NDHelpers::parseNullableByteConvertible<State>(query.value(9));
        if (state.isCorrupted()) return std::nullopt;

        return NDWireDetails::Config::FullWireArbitraryRecord<State>{
            id,
            *coreId,
            *origin,
            *target,
            QPointF(query.value(3).toReal(), query.value(4).toReal()),
            QPointF(query.value(7).toReal(), query.value(8).toReal()),
            state.value
        };
    }

    template<NDConcepts::ByteConvertible State>
    inline std::optional<QList<NDWireDetails::Config::FullWireArbitraryRecord<State>>> getContributorWireArbitrary(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
        QList<NDWireDetails::Config::FullWireArbitraryRecord<State>> list;

        query.prepare(R"(
            SELECT wa.id, wa.core_id, wa.origin_pin_id, wa.origin_widget_id, wa.origin_canvas_hint_x, wa.origin_canvas_hint_y, wa.target_pin_id, wa.target_widget_id, wa.target_canvas_hint_x, wa.target_canvas_hint_y, wa.state
            FROM wire_arbitrary wa
            INNER JOIN wire_core cr ON wa.core_id = cr.id
            WHERE cr.contributor_id = :contributor_id;
        )");
        query.bindValue(":contributor_id", Utility::UUID::uuidToBytes(contributorId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getContributorWireArbitrary query:" << query.lastError().text();
            return std::nullopt;
        }

        auto makeVertex = [](const QVariant& pin, const QVariant& widget) -> std::optional<NDWireDetails::Config::Vertex> {
            if (pin.isNull() == widget.isNull())
                return std::nullopt;

            if (!pin.isNull()) {
                const auto id = Utility::UUID::bytesToUuid(pin.toByteArray());
                if (!id) return std::nullopt;
                return NDWireDetails::Config::PinVertex{ *id };
            }
            else if (!widget.isNull()) {
                const auto id = Utility::UUID::bytesToUuid(widget.toByteArray());
                if (!id) return std::nullopt;
                return NDWireDetails::Config::WidgetVertex{ *id };
            }
            return std::nullopt;
            };

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto coreId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());

            const std::optional<NDWireDetails::Config::Vertex> origin = makeVertex(query.value(2), query.value(3));
            const std::optional<NDWireDetails::Config::Vertex> target = makeVertex(query.value(6), query.value(7));

            const NDHelpers::NullableField<State> state = NDHelpers::parseNullableByteConvertible<State>(query.value(10));

            if (!id || !coreId || !origin || !target || state.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireDetails::Config::FullWireArbitraryRecord<State>{
                *id,
                *coreId,
                *origin,
                *target,
                QPointF(query.value(4).toReal(), query.value(5).toReal()),
                QPointF(query.value(8).toReal(), query.value(9).toReal()),
                state.value
            });
        }
        return list;
    }

    template<NDConcepts::ByteConvertible State>
    inline std::optional<QList<NDWireDetails::Config::FullWireArbitraryRecord<State>>> getAllWireArbitrary(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
        QList<NDWireDetails::Config::FullWireArbitraryRecord<State>> list;

        query.prepare(R"(
            SELECT wa.id, wa.core_id, wa.origin_pin_id, wa.origin_widget_id, wa.origin_canvas_hint_x, wa.origin_canvas_hint_y, wa.target_pin_id, wa.target_widget_id, wa.target_canvas_hint_x, wa.target_canvas_hint_y, wa.state
            FROM wire_arbitrary wa
            INNER JOIN wire_core cr ON wa.core_id = cr.id
            INNER JOIN widget_contributor c ON cr.contributor_id = c.id
            WHERE c.source_id = :source_id;
        )");
        query.bindValue(":source_id", Utility::UUID::uuidToBytes(sourceId));

        if (!query.exec()) {
            qCritical() << "Failed to execute getAllWireArbitrary query:" << query.lastError().text();
            return std::nullopt;
        }

        auto makeVertex = [](const QVariant& pin, const QVariant& widget) -> std::optional<NDWireDetails::Config::Vertex> {
            if (pin.isNull() == widget.isNull())
                return std::nullopt;

            if (!pin.isNull()) {
                const auto id = Utility::UUID::bytesToUuid(pin.toByteArray());
                if (!id) return std::nullopt;
                return NDWireDetails::Config::PinVertex{ *id };
            }
            else if (!widget.isNull()) {
                const auto id = Utility::UUID::bytesToUuid(widget.toByteArray());
                if (!id) return std::nullopt;
                return NDWireDetails::Config::WidgetVertex{ *id };
            }
            return std::nullopt;
            };

        while (query.next()) {
            const auto id = Utility::UUID::bytesToUuid(query.value(0).toByteArray());
            const auto coreId = Utility::UUID::bytesToUuid(query.value(1).toByteArray());

            const std::optional<NDWireDetails::Config::Vertex> origin = makeVertex(query.value(2), query.value(3));
            const std::optional<NDWireDetails::Config::Vertex> target = makeVertex(query.value(6), query.value(7));

            const NDHelpers::NullableField<State> state = NDHelpers::parseNullableByteConvertible<State>(query.value(10));

            if (!id || !coreId || !origin || !target || state.isCorrupted()) {
                if (continueAtFail)
                    continue;
                else
                    return std::nullopt;
            }

            list.append(NDWireDetails::Config::FullWireArbitraryRecord<State>{
                *id,
                *coreId,
                *origin,
                *target,
                QPointF(query.value(4).toReal(), query.value(5).toReal()),
                QPointF(query.value(8).toReal(), query.value(9).toReal()),
                state.value
            });
        }
        return list;
    }
}