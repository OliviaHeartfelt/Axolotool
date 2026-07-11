#pragma once

#include "../../NDConcepts.h"

namespace NDWireDetails::Config {

    // 0. Wire Core
    struct FullWireCoreRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        std::optional<muuid::uuid> styleId;
        std::optional<muuid::uuid> dataId;
        QString name;
    };
    struct CreateWireCoreRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        std::optional<muuid::uuid> styleId;
        std::optional<muuid::uuid> dataId;
        QString name;
    };
    struct UpdateWireCoreRecord {
        std::optional<muuid::uuid> styleId;
        std::optional<muuid::uuid> dataId;
        std::optional<QString>     name;
    };

    // 1. Wire Temporary
    struct FullWireTemporaryRecord {
        muuid::uuid id;
        muuid::uuid coreId;
    };
    struct CreateWireTemporaryRecord {
        muuid::uuid id;
        muuid::uuid coreId;
    };

    // 2. Wire Pins
    template<NDConcepts::ByteConvertible State>
    struct FullWirePinRecord {
        muuid::uuid id;
        muuid::uuid coreId;
        muuid::uuid originId;
        muuid::uuid targetId;
        QPointF originHintPos;
        QPointF targetHintPos;
        std::optional<State> state;
    };
    template<NDConcepts::ByteConvertible State>
    struct CreateWirePinRecord {
        muuid::uuid id;
        muuid::uuid coreId;
        muuid::uuid originId;
        muuid::uuid targetId;
        QPointF originHintPos;
        QPointF targetHintPos;
        std::optional<State> state;
    };
    template<NDConcepts::ByteConvertible State>
    struct UpdateWirePinRecord {
        std::optional<QPointF> originHintPos;
        std::optional<QPointF> targetHintPos;
        std::optional<State>   state;
    };

    // 3. Wire Widgets
    template<NDConcepts::ByteConvertible State>
    struct FullWireWidgetsRecord {
        muuid::uuid id;
        muuid::uuid coreId;
        muuid::uuid originId;
        muuid::uuid targetId;
        QPointF originHintPos;
        QPointF targetHintPos;
        std::optional<State> state;
    };
    template<NDConcepts::ByteConvertible State>
    struct CreateWireWidgetsRecord {
        muuid::uuid id;
        muuid::uuid coreId;
        muuid::uuid originId;
        muuid::uuid targetId;
        QPointF originHintPos;
        QPointF targetHintPos;
        std::optional<State> state;
    };
    template<NDConcepts::ByteConvertible State>
    struct UpdateWireWidgetsRecord {
        std::optional<QPointF> originHintPos;
        std::optional<QPointF> targetHintPos;
        std::optional<State>   state;
    };

    //4. Wire Arbitrary
    struct PinVertex {
        muuid::uuid id;
    };
    struct WidgetVertex {
        muuid::uuid id;
    };
    using Vertex = std::variant<PinVertex, WidgetVertex>;

    template<NDConcepts::ByteConvertible State>
    struct FullWireArbitraryRecord {
        muuid::uuid id;
        muuid::uuid coreId;
        Vertex origin;
        Vertex target;
        QPointF originHintPos;
        QPointF targetHintPos;
        std::optional<State> state;
    };
    template<NDConcepts::ByteConvertible State>
    struct CreateWireArbitraryRecord {
        muuid::uuid id;
        muuid::uuid coreId;
        Vertex origin;
        Vertex target;
        QPointF originHintPos;
        QPointF targetHintPos;
        std::optional<State> state;
    };
    template<NDConcepts::ByteConvertible State>
    struct UpdateWireArbitraryRecord {
        std::optional<QPointF> originHintPos;
        std::optional<QPointF> targetHintPos;
        std::optional<State>   state;
    };
}