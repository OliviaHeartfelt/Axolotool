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
        std::optional<muuid::uuid> id =            std::nullopt;
        std::optional<muuid::uuid> contributorId = std::nullopt;
        std::optional<muuid::uuid> styleId =       std::nullopt;
        std::optional<muuid::uuid> dataId =        std::nullopt;
        std::optional<QString> name =              std::nullopt;
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

    // 2. Wire
    struct FullWireRecord {
        muuid::uuid id;
        muuid::uuid coreId;
        muuid::uuid originId;
        muuid::uuid targetId;
        QPointF originHintPos;
        QPointF targetHintPos;
        std::optional<std::vector<uint8_t>> state;
    };
    struct CreateWireRecord {
        muuid::uuid id;
        muuid::uuid coreId;
        muuid::uuid originId;
        muuid::uuid targetId;
        QPointF originHintPos;
        QPointF targetHintPos;
        std::optional<std::vector<uint8_t>> state = std::nullopt;
    };
    struct FactoryWireRecord {
        muuid::uuid originId;
        muuid::uuid targetId;
        QPointF originHintPos;
        QPointF targetHintPos;
    };
    struct UpdateWireRecord {
        std::optional<muuid::uuid> id =        std::nullopt;
        std::optional<muuid::uuid> coreId =    std::nullopt;
        std::optional<QPointF> originHintPos = std::nullopt;
        std::optional<QPointF> targetHintPos = std::nullopt;
        std::variant<std::monostate, std::optional<std::vector<uint8_t>>> state = std::monostate{};
    };
}