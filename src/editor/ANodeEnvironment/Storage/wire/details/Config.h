#pragma once

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