#pragma once

#include "../../NDConcepts.h"
#include "../../NDParser.h"

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

        static std::optional<CreateGlobalSourceRecord> Parse(const QJsonObject& obj) {
            auto optID = NDParser::parse<muuid::uuid>(obj, "id");
            if (!optID) return std::nullopt;

            auto optName = NDParser::parse<QString>(obj, "name");
            if (!optName) return std::nullopt;

            auto optDsc = NDParser::parse<QString>(obj, "dsc", true);

            return CreateGlobalSourceRecord{
                .id = *optID,
                .name = *optName,
                .dsc = optDsc
            };
        }
    };
    struct UpdateGlobalSourceRecord {
        std::optional<muuid::uuid> id = std::nullopt;
        std::optional<QString> name =   std::nullopt;
        std::variant<std::monostate, std::optional<QString>> dsc = std::monostate{};
    };
}