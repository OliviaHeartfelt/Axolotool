#pragma once

#include "registry_base/RGBaseRegistry.h"
#include "details/Context.h"
#include "details/RegistryTraits.h"

#include "../Storage/ANodeEnvDB.h"

#include "../View/node/details/Context.h"
#include "../View/pin/details/Context.h"
#include "../View/pin/details/Concepts.h"
#include "../View/widget/details/Context.h"
#include "../View/wire/details/Context.h"

namespace ARegistry {
    template<typename T>
    using RegistryT = RGBaseRegistry::BaseRegistry<muuid::uuid, T>;

    struct NodeRegisters {
        RegistryT<ANodeEnvDB::Config::Node::FullNodeCoreRecord> nodeCoreRegistry;
        RegistryT<ANodeEnvDB::Config::NodeSource::FullNodeTypeRecord>  nodeTypeRegistry;
        RegistryT<ANodeEnvDB::Config::NodeSource::FullNodeDataRecord>  nodeDataRegistry;

        RegistryT<ANodeEnvDB::Config::Pin::CompletePinCore> pinCoreRegistry;
        RegistryT<ANodeEnvDB::Config::PinSource::FullPinFlowRecord>  pinFlowRegistry;
        RegistryT<ANodeEnvDB::Config::PinSource::FullPinTypeRecord>  pinTypeRegistry;
        RegistryT<ANodeEnvDB::Config::PinSource::FullPinStyleRecord> pinStyleRegistry;

        RegistryT<ANodeEnvDB::Config::Widget::FullWidgetCoreRecord> widgetCoreRegistry;
        RegistryT<ANodeEnvDB::Config::WidgetSource::FullWidgetTypeRecord> widgetTypeRegistry;
        RegistryT<ANodeEnvDB::Config::WidgetSource::FullWidgetDataRecord> widgetDataRegistry;
    };

    struct NodeViewRegisters {
        RegistryT<QGraphicsItem*> nodeViewRegistry;
        RegistryT<QGraphicsItem*> pinViewRegistry;
    };

    struct NodeFunctionRegistry {
        using CellFactory = std::function<std::vector<ARegistryDetails::Context::CreateCellRecord>(const muuid::uuid& nodeId)>;
        RegistryT<CellFactory> cellFactoryRegistry;

        using PinConfigurator = std::function<bool(QGraphicsRectItem* nodeCell, QGraphicsItem* pin, VWPinDetails::Context::PinContext pinContext, const std::optional<QString> text)>;
        RegistryT<PinConfigurator> pinConfiguratorRegistry;

        using WidgetFactory = std::function<QGraphicsItem*(VWWidgetDetails::Context::WidgetContext widgetContext, const std::optional<QString> text)>;
        RegistryT<WidgetFactory> widgetFactoryRegistry;

        using CellFallbackConfigurator = std::function<bool(QGraphicsItem* nodeCell, const std::optional<QString> text)>;
        RegistryT<CellFallbackConfigurator> cellFallbackConfiguratorRegistry;
    };


    struct WireRegisters {
        RegistryT<ANodeEnvDB::Config::Wire::FullWireCoreRecord> wireCoreRegistry;
        RegistryT<ANodeEnvDB::Config::WireSource::FullWireDataRecord> wireDataRegistry;
        RegistryT<ANodeEnvDB::Config::WireSource::FullWireStyleRecord> wireStyleRegistry;
    };

    struct WireViewRegisters {

        RegistryT<QGraphicsItem*> wireViewRegistry;
    };

    struct WireFunctionRegistry {
        using WireConfigurator = std::function<bool(QGraphicsItem* wire, VWWireDetails::Context::WireContext wireContext)>;
        RegistryT<WireConfigurator> wireConfiguratorRegistry;

        RGBaseRegistry::BaseRegistry<
            std::pair<muuid::uuid, muuid::uuid>, 
            muuid::uuid,
            ARegistryDetails::RegistryTraits::WirePairHash,
            ARegistryDetails::RegistryTraits::WirePairEqual
        > asymmetricWireMap;

        RGBaseRegistry::BaseRegistry<
            muuid::uuid,
            muuid::uuid
        > symmetricWireMap;
    };

    struct Registry {
        NodeRegisters node;
        NodeViewRegisters nodeView;
        NodeFunctionRegistry nodeFunction;

        WireRegisters wire;
        WireViewRegisters wireView;
        WireFunctionRegistry wireFunction;
    };
}