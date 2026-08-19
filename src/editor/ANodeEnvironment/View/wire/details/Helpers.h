#pragma once

#include "../../../Registry/ARegistry.h"
#include "../../../Storage/ANodeEnvDB.h"
#include "../../pin/details/Concepts.h"
#include "Context.h"
#include "WireItem.h"

namespace VWWireDetails::Helpers {

    inline bool useConfigurator(ARegistry::Registry* registry, ANodeEnvDB::ANodeEnvDB* nodeEnvDB, WireItem::WireItem* wire, const muuid::uuid& wireCoreId) {        
        if (!registry || !nodeEnvDB || !wire) return false;

        auto wireCoreOpt = registry->wire.wireCoreRegistry.at(wireCoreId);

        if (!wireCoreOpt) {
            ANodeEnvDB::Helpers::useQuery(nodeEnvDB->getPool(), [&](QSqlQuery& guery) {
                wireCoreOpt = nodeEnvDB->wire.getWireCore(guery, wireCoreId);
                });

            if (wireCoreOpt) {
                registry->wire.wireCoreRegistry.insert(wireCoreId, *wireCoreOpt);
            }
        }

        if (!wireCoreOpt || !wireCoreOpt->styleId) return false;

        auto fn = registry->wireFunction.wireConfiguratorRegistry.at(wireCoreOpt->visualFactoryId);
        if (!fn) return false;

        Context::WireContext wireContext{
            .style = std::nullopt,
            .data = std::nullopt
        };
        wireContext.name = wireCoreOpt->name;

        if (wireCoreOpt->styleId) {
            auto wireStyleOpt = registry->wire.wireStyleRegistry.at(*wireCoreOpt->styleId);
            if (!wireStyleOpt) {
                ANodeEnvDB::Helpers::useQuery(nodeEnvDB->getPool(), [&](QSqlQuery& guery) {
                    wireStyleOpt = nodeEnvDB->wireSource.getWireStyle(guery, *wireCoreOpt->styleId);
                    });

                if (wireStyleOpt) {
                    registry->wire.wireStyleRegistry.insert(*wireCoreOpt->styleId, *wireStyleOpt);
                }
            }
            if (!wireStyleOpt) return false;

            wireContext.style = Context::WireStyleContext{
                .name = wireStyleOpt->name,
                .color = wireStyleOpt->color,
                .wireThickness = wireStyleOpt->wireThickness,
                .metadata = wireStyleOpt->metadata
            };
        }

        if (wireCoreOpt->dataId) {
            auto wiredata = registry->wire.wireDataRegistry.at(*wireCoreOpt->dataId);
            if (!wiredata) {
                ANodeEnvDB::Helpers::useQuery(nodeEnvDB->getPool(), [&](QSqlQuery& guery) {
                    wiredata = nodeEnvDB->wireSource.getWireData(guery, *wireCoreOpt->dataId);
                    });

                if (wiredata) {
                    registry->wire.wireDataRegistry.insert(*wireCoreOpt->dataId, *wiredata);
                }
            }
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