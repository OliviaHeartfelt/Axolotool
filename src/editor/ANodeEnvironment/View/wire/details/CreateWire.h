#pragma once

#include "WireItem.h"
#include "Helpers.h"
#include "../../pin/details/Concepts.h"
#include "../../cell/details/Concepts.h"
#include "../../../Registry/ARegistry.h"
#include "../../../Storage/ANodeEnvDB.h"

namespace VWWireDetails::CreateWire {

    template<VWPinDetails::Concepts::PinItemConcept<WireItem::WireItem> T>
    inline WireItem::WireItem* createPermanentWire(
        T* orininPin,
        T* targetPin,
        ARegistry::Registry* registry,
        ANodeEnvDB::ANodeEnvDB* nodeEnvDB,
        std::optional<muuid::uuid> id
    ) {
        if (!orininPin || !orininPin->scene() || !targetPin || !registry || !nodeEnvDB) return nullptr;

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
            coreIdOpt,
            id
        );
        if (!wire) return nullptr;

        if (!coreIdOpt)
            return Helpers::useFallback(wire);

        if (!Helpers::useConfigurator(registry, nodeEnvDB, wire, *coreIdOpt))
            return Helpers::useFallback(wire);

        registry->wireView.wireViewRegistry.addVisible(wire->id(), wire);

        orininPin->scene()->addItem(wire);

        orininPin->registerWire(wire);
        targetPin->registerWire(wire);

        qDebug() << "> Wire created! #Wires:" << registry->wireView.wireViewRegistry.sizeVisible() - 1 << "->" << registry->wireView.wireViewRegistry.sizeVisible();

        return wire;
    }

    template<VWPinDetails::Concepts::PinItemConcept<WireItem::WireItem> Pin, WVCellDetails::Concepts::CellItem Cell>
    inline WireItem::WireItem* createPermanentWire(
        muuid::uuid orininCellId,
        muuid::uuid targetCellId,
        ARegistry::Registry* registry,
        ANodeEnvDB::ANodeEnvDB* nodeEnvDB,
        std::optional<muuid::uuid> id
    ) {
        if (!registry || !nodeEnvDB) return nullptr;


        const auto originCellOpt = registry->nodeView.cellViewRegistry.findVisible(orininCellId);
        if (!originCellOpt) return nullptr;

        auto* originCellPtr = dynamic_cast<Cell*>(*originCellOpt);
        if (!originCellPtr) return nullptr;

        Pin* originPinPtr = nullptr;
        for (auto* originChildren : originCellPtr->childItems()) {
            auto* ptr = dynamic_cast<Pin*>(originChildren);
            if (!ptr) continue;
            originPinPtr = ptr;
        }
        if (!originPinPtr) return nullptr;


        const auto targetCellOpt = registry->nodeView.cellViewRegistry.findVisible(targetCellId);
        if (!targetCellOpt) return nullptr;

        auto* targetCellPtr = dynamic_cast<Cell*>(*targetCellOpt);
        if (!targetCellPtr) return nullptr;

        Pin* targetPinPtr = nullptr;
        for (auto* targetChildren : targetCellPtr->childItems()) {
            auto* ptr = dynamic_cast<Pin*>(targetChildren);
            if (!ptr) continue;
            targetPinPtr = ptr;
        }
        if (!targetPinPtr) return nullptr;

        return createPermanentWire(originPinPtr, targetPinPtr, registry, nodeEnvDB, id);
    }
}