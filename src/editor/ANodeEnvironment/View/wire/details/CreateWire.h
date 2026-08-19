#pragma once

#include "WireItem.h"
#include "Helpers.h"
#include "../../pin/details/Concepts.h"
#include "../../../Registry/ARegistry.h"
#include "../../../Storage/ANodeEnvDB.h"

namespace VWWireDetails::CreateWire {

    template<VWPinDetails::Concepts::PinItemConcept T>
    inline WireItem::WireItem* createPermanentWire(
        T* orininPin,
        T* targetPin,
        ARegistry::Registry* registry,
        ANodeEnvDB::ANodeEnvDB* nodeEnvDB
    ) {
        if (!orininPin || !targetPin || !registry || !nodeEnvDB) return nullptr;

        const auto originData = Helpers::resolvePinData(*registry, orininPin);
        if (!originData) return nullptr;

        const auto targetData = Helpers::resolvePinData(*registry, targetPin);
        if (!targetData) return nullptr;

        std::optional<muuid::uuid> coreIdOpt = std::nullopt;
        if (originData->typeId && targetData->typeId) {
            coreIdOpt = Helpers::resolveWireCore(*registry, *originData->typeId, *targetData->typeId);
        }

        auto* wire = new WireItem::WireItem(
            originData->pin,
            targetData->pin,
            originData->degree,
            targetData->degree,
            coreIdOpt
        );
        if (!wire) return nullptr;

        if (!coreIdOpt)
            return Helpers::useFallback(wire);

        if (!Helpers::useConfigurator(registry, nodeEnvDB, wire, *coreIdOpt))
            return Helpers::useFallback(wire);

        return wire;
    }

    template<VWPinDetails::Concepts::PinItemConcept T>
    inline WireItem::WireItem* createPermanentWire(
        muuid::uuid orininPinId,
        muuid::uuid targetPinId,
        ARegistry::Registry* registry,
        ANodeEnvDB::ANodeEnvDB* nodeEnvDB
    ) {
        if (!registry || !nodeEnvDB) return nullptr;

        const auto originPinPtrOpt = registry->nodeView.pinViewRegistry.at(orininPinId);
        if (!originPinPtrOpt) return nullptr;

        T* originPinPtr = dynamic_cast<T*>(*originPinPtrOpt);
        if (!originPinPtr) return nullptr;

        const auto targetPinPtrOpt = registry->nodeView.pinViewRegistry.at(targetPinId);
        if (!targetPinPtrOpt) return nullptr;

        T* targetPinPtr = dynamic_cast<T*>(*originPinPtrOpt);
        if (!targetPinPtr) return nullptr;

        return createPermanentWire(originPinPtr, targetPinPtr, registry, nodeEnvDB);
    }
}