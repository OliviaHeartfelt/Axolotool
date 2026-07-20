#pragma once

#include "../../NDConcepts.h"

namespace NDWidgetDetails::Config {

    // Widget Core
    struct FullWidgetCoreRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        std::optional<muuid::uuid> typeId;
        std::optional<muuid::uuid> dataId;
    };
    struct CreateWidgetCoreRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        std::optional<muuid::uuid> typeId = std::nullopt;
        std::optional<muuid::uuid> dataId = std::nullopt;
    };
    struct UpdateWidgetCoreRecord {
        std::optional<muuid::uuid> id =            std::nullopt;
        std::optional<muuid::uuid> contributorId = std::nullopt;
        std::variant<std::monostate, std::optional<muuid::uuid>> typeId = std::monostate{};
        std::variant<std::monostate, std::optional<muuid::uuid>> dataId = std::monostate{};
    };

    // Widget
    struct FullWidgetRecord {
        muuid::uuid id;
        muuid::uuid coreId;
        std::optional<std::vector<uint8_t>> state;
        qreal w;
        qreal h;
    };
    struct CreateWidgetRecord {
        muuid::uuid id;
        muuid::uuid coreId;
        std::optional<std::vector<uint8_t>> state = std::nullopt;
        std::optional<qreal> w = std::nullopt;
        std::optional<qreal> h = std::nullopt;
    };
    struct FactoryWidgetRecord {
        std::optional<qreal> w = std::nullopt;
        std::optional<qreal> h = std::nullopt;
    };
    struct UpdateWidgetRecord {
        std::optional<muuid::uuid> id =     std::nullopt;
        std::optional<muuid::uuid> coreId = std::nullopt;
        std::variant<std::monostate, std::optional<std::vector<uint8_t>>> state = std::monostate{};
        std::optional<qreal> width  = std::nullopt;
        std::optional<qreal> height = std::nullopt;
    };
}