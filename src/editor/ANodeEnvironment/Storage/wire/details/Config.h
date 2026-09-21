#pragma once

#include "../../NDParser.h"
#include "../../NDConcepts.h"

namespace NDWireDetails::Config {

    // 0. Wire Core
    struct FullWireCoreRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        muuid::uuid visualFactoryId;
        QString name;
        std::optional<muuid::uuid> styleId;
        std::optional<muuid::uuid> dataId;
    };
    struct CreateWireCoreRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        muuid::uuid visualFactoryId;
        QString name;
        std::optional<muuid::uuid> styleId;
        std::optional<muuid::uuid> dataId;

        static std::optional<CreateWireCoreRecord> Parse(const QJsonObject& obj) {

            auto optID = NDParser::parse<muuid::uuid>(obj, "id");
            if (!optID) return std::nullopt;

            auto contributorId = NDParser::parse<muuid::uuid>(obj, "contributorId", true);
            if (!contributorId) return std::nullopt;

            auto visualFactoryId = NDParser::parse<muuid::uuid>(obj, "visualFactoryId", true);
            if (!visualFactoryId) return std::nullopt;

            auto name = NDParser::parse<QString>(obj, "name");
            if (!name) return std::nullopt;

            auto styleId = NDParser::parse<muuid::uuid>(obj, "styleId");
            auto dataId = NDParser::parse<muuid::uuid>(obj, "dataId");

            return CreateWireCoreRecord{
                .id = *optID,
                .contributorId = *contributorId,
                .visualFactoryId = *visualFactoryId,
                .name = *name,
                .styleId = styleId,
                .dataId = dataId
            };
        }
    };
    struct UpdateWireCoreRecord {
        std::optional<muuid::uuid> id =              std::nullopt;
        std::optional<muuid::uuid> contributorId =   std::nullopt;
        std::optional<muuid::uuid> visualFactoryId = std::nullopt;
        std::optional<QString> name =                std::nullopt;
        std::variant<std::monostate, std::optional<muuid::uuid>> styleId = std::monostate{};
        std::variant<std::monostate, std::optional<muuid::uuid>> dataId =  std::monostate{};
    };

    // 1. Wire Temporary
    struct FullWireTemporaryRecord {
        muuid::uuid id;
        muuid::uuid coreId;
    };
    struct CreateWireTemporaryRecord {
        muuid::uuid id;
        muuid::uuid coreId;

        static std::optional<CreateWireTemporaryRecord> Parse(const QJsonObject& obj) {

            auto optID = NDParser::parse<muuid::uuid>(obj, "id");
            if (!optID) return std::nullopt;

            auto coreId = NDParser::parse<muuid::uuid>(obj, "coreId", true);
            if (!coreId) return std::nullopt;

            return CreateWireTemporaryRecord{
                .id = *optID,
                .coreId = *coreId
            };
        }
    };

    // 2. Wire
    struct CreateWireRecord {
        muuid::uuid id;
        muuid::uuid coreId;
        muuid::uuid originId;
        muuid::uuid targetId;
        QPointF originHintPos;
        QPointF targetHintPos;
        std::optional<std::vector<uint8_t>> state = std::nullopt;

        static std::optional<CreateWireRecord> Parse(const QJsonObject& obj) {

            auto optID = NDParser::parse<muuid::uuid>(obj, "id");
            if (!optID) return std::nullopt;

            auto coreId = NDParser::parse<muuid::uuid>(obj, "coreId", true);
            if (!coreId) return std::nullopt;

            auto originId = NDParser::parse<muuid::uuid>(obj, "originId", true);
            if (!originId) return std::nullopt;

            auto targetId = NDParser::parse<muuid::uuid>(obj, "targetId", true);
            if (!targetId) return std::nullopt;

            auto origin_x = NDParser::parse<double>(obj, "origin_x", true);
            if (!origin_x) return std::nullopt;

            auto origin_y = NDParser::parse<double>(obj, "origin_y", true);
            if (!origin_y) return std::nullopt;

            auto target_x = NDParser::parse<double>(obj, "target_x", true);
            if (!target_x) return std::nullopt;

            auto target_y = NDParser::parse<double>(obj, "target_y", true);
            if (!target_y) return std::nullopt;

            return CreateWireRecord{
                .id = *optID,
                .coreId = *coreId,
                .originId = *originId,
                .targetId = *targetId,
                .originHintPos = QPointF(*origin_x, *origin_y),
                .targetHintPos = QPointF(*target_x, *target_y),
            };
        }
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

    struct FullWireRecord {
        muuid::uuid id;
        muuid::uuid coreId;
        muuid::uuid originId;
        muuid::uuid targetId;
        QPointF originHintPos;
        QPointF targetHintPos;
        std::optional<std::vector<uint8_t>> state;

        static CreateWireRecord toCreate(const FullWireRecord& fullRecord) {
            return CreateWireRecord{
                .id = fullRecord.id,
                .coreId = fullRecord.coreId,
                .originId = fullRecord.originId,
                .targetId = fullRecord.targetId,
                .originHintPos = fullRecord.originHintPos,
                .targetHintPos = fullRecord.targetHintPos,
                .state = std::nullopt
            };
        }
        static UpdateWireRecord toUpdate(const FullWireRecord& fullRecord) {
            return UpdateWireRecord{
                .id = std::nullopt,
                .coreId = std::nullopt,
                .originHintPos = fullRecord.originHintPos,
                .targetHintPos = fullRecord.targetHintPos,
                .state = std::monostate{}
            };
        }
    };
}