#pragma once

#include "../../NDConcepts.h"

namespace NDGlobalSourceDetails::Config {

    struct FullGlobalSourceRecord {
        muuid::uuid id;
        QString name;
        std::optional<QString> dsc;
    };
    struct CreateGlobalSourceRecord {
        muuid::uuid id;
        QString name;
        std::optional<QString> dsc = std::nullopt;
    };
    struct UpdateGlobalSourceRecord {
        std::optional<muuid::uuid> id = std::nullopt;
        std::optional<QString> name =   std::nullopt;
        std::variant<std::monostate, std::optional<QString>> dsc = std::monostate{};
    };
}