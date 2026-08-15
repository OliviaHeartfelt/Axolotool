#pragma once

namespace VWWidgetDetails::Context {

    struct WidgetContextItem {
        std::optional<std::string> name = std::nullopt;
        std::optional<std::vector<uint8_t>> value = std::nullopt;
    };
    struct WidgetContext {
        WidgetContextItem metadata;
        WidgetContextItem data;
        std::optional<std::vector<uint8_t>> state;
    };
}