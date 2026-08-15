#pragma once

#include "../../../Registry/ARegistry.h"
#include "../../../Storage/ANodeEnvDB.h"
#include "../../pin/details/Concepts.h"
#include "Context.h"
#include "WireItem.h"

namespace VWWireDetails::Helpers {

    inline bool useConfigurator(ARegistry::Registry& registry, WireItem::WireItem* wire, const muuid::uuid& wireCoreId) {
        if (!wire) return false;

        const auto wireCore = registry.wire.wireCoreRegistry.at(wireCoreId);
        if (!wireCore) return false;

        auto fn = registry.wireFunction.wireConfiguratorRegistry.at(wireCore->visualFactoryId);
        if (!fn) return false;

        Context::WireContext wireContext{
            .style = std::nullopt,
            .data = std::nullopt
        };
        wireContext.name = wireCore->name;

        if (wireCore->styleId) {
            const auto wireStyleOpt = registry.wire.wireStyleRegistry.at(*wireCore->styleId);
            if (!wireStyleOpt) return false;

            wireContext.style = Context::WireStyleContext{
                .name = wireStyleOpt->name,
                .color = wireStyleOpt->color,
                .wireThickness = wireStyleOpt->wireThickness,
                .metadata = wireStyleOpt->metadata
            };
        }

        if (wireCore->dataId) {
            const auto wiredata = registry.wire.wireDataRegistry.at(*wireCore->dataId);
            if (!wiredata) return false;

            wireContext.data = Context::WireDataContext{
                .name = wiredata->name,
                .data = wiredata->data
            };
        }

        return (*fn)(wire, std::move(wireContext));
    }

    inline WireItem::WireItem* useFallback(WireItem::WireItem* wire) {
        if (!wire) return nullptr;

        wire->setPen(QPen(Qt::lightGray, 2, Qt::SolidLine));
        wire->setZValue(-1);
        return wire;
    }

    inline std::optional<muuid::uuid> resolveWireCore(ARegistry::Registry& registry, const muuid::uuid& originTypeId, const muuid::uuid& targetTypeId) {
        if (originTypeId == targetTypeId) {
            if (auto wireCoreId = registry.wireFunction.symmetricWireMap.at(originTypeId)) {
                return wireCoreId;
            }
        }
        else {
            if (auto wireCoreId = registry.wireFunction.asymmetricWireMap.at({ originTypeId, targetTypeId })) {
                return wireCoreId;
            }
        }
        return std::nullopt;
    }

    template<VWPinDetails::Concepts::PinItemConcept T>
    std::optional<Context::FactoryPinData> resolvePinData(ARegistry::Registry& registry, T* pin) {
        if (!pin) return std::nullopt;

        const auto pinCoreOpt = registry.node.pinCoreRegistry.at(pin->coreId());
        if (!pinCoreOpt) return std::nullopt;

        if (!pinCoreOpt->flowId) return std::nullopt;
        const auto pinFlowOpt = registry.node.pinFlowRegistry.at(*pinCoreOpt->flowId);
        if (!pinFlowOpt) return std::nullopt;

        return Context::FactoryPinData{
            .pin = pin,
            .typeId = pinCoreOpt->typeId,
            .degree = pinFlowOpt->degree
        };
    }

    template<VWPinDetails::Concepts::PinItemConcept T>
    T* resolvePinPtr(std::unordered_map<muuid::uuid, T*>& pinMap, const muuid::uuid& pinId) {
        const auto pinIt = pinMap.find(pinId);
        if (pinIt == pinMap.end() || !pinIt->second) return nullptr;

        return pinIt->second;
    }
}