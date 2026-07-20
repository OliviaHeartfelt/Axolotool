#pragma once

#include "../../Storage/ANodeEnvDB.h"

namespace MockData {

    namespace Color {
        inline QColor Execution = QColor::fromString("#ffffffff"); // Pure White
        inline QColor Class =     QColor::fromString("#ff00a6f2"); // Sky Blue 
        inline QColor Object =    QColor::fromString("#ff00a6f2"); // Sky Blue 

        inline QColor Bool =  QColor::fromString("#ff910000"); // Crimson
        inline QColor Byte =  QColor::fromString("#ff006d64"); // Forest Teal
        inline QColor Int =   QColor::fromString("#ff22e0ac"); // Mint Green
        inline QColor Int64 = QColor::fromString("#ffaae1ad"); // Pastel Mint

        inline QColor Float =  QColor::fromString("#ff9dfc43"); // Marigold 
        inline QColor Vector = QColor::fromString("#fffbc627"); // Lime Green

        inline QColor Name =   QColor::fromString("#ffca80fb"); // Lavender
        inline QColor String = QColor::fromString("#fffd00d1"); // Magenta
    }

    namespace Helper {
        inline auto make_uuid(const std::string hexStr) {
            auto opt = muuid::uuid::from_chars(hexStr);
            assert(opt && "Fix mock UUID string typo!");
            return *opt;
        };
    }

    void injectMockPluginData(ANodeEnvDB::ANodeEnvDB* nodeEnvDB) {
        using namespace ANodeEnvDB::Config;
        using namespace ANodeEnvDB::Helpers;

        useTransaction(nodeEnvDB->getPool(), [&](QSqlQuery& query) {

            // --------------------------------------------------------------------------------------------------------------------------------
            // 0. Global Source
            // --------------------------------------------------------------------------------------------------------------------------------
            const auto globalSourceId_v = MockData::Helper::make_uuid("019f5d11-0c09-741c-a9ef-b59f7ec0b171");
            nodeEnvDB->globalSource.createGlobalSource(query, GlobalSource::CreateGlobalSourceRecord{
                .id = globalSourceId_v,
                .name = "Standard",
                .dsc = ""
            });

            // --------------------------------------------------------------------------------------------------------------------------------
            // 1.1. Node Source
            // --------------------------------------------------------------------------------------------------------------------------------
            const auto nodeSourceId_v = MockData::Helper::make_uuid("019f5d11-0c1a-741c-a9ef-b59f7ec0b222");
            nodeEnvDB->nodeSource.createNodeSource(query, NodeSource::CreateNodeSourceRecord{
                .id = nodeSourceId_v,
                .globalSourceId = globalSourceId_v,
                .name = "Standard::NodeSource"
            });
            const auto nodeContributorId_v = MockData::Helper::make_uuid("019f5d2f-98dc-7c4d-9986-487e04ec141b");
            nodeEnvDB->nodeSource.createNodeContributor(query, NodeSource::CreateNodeContributorRecord{
                .id = nodeContributorId_v,
                .sourceId = nodeSourceId_v,
                .name = "Standard::Contributor[0]"
            });

            // Node Type Metadata
            class NodeTypeMetadata {
            public:
                static std::optional<NodeTypeMetadata> bytesToClass(std::span<const uint8_t> bytes) { if (bytes.empty()) return std::nullopt; return NodeTypeMetadata{}; }
                std::vector<uint8_t> classToBytes() const { return std::vector<uint8_t>{}; }
            };
            const auto nodeTypeId_v = MockData::Helper::make_uuid("019f65ff-1fe9-7ac3-931a-f2c225c5267c");
            const NodeTypeMetadata nodeMetadata;
            nodeEnvDB->nodeSource.createNodeType(query, NodeSource::CreateNodeTypeRecord{
                .id = nodeTypeId_v,
                .contributorId = nodeContributorId_v,
                .name = "Standard::Type[0]",
                .metadata = nodeMetadata.classToBytes()
            });

            // Node Data
            class NodeData {
            public:
                static std::optional<NodeData> bytesToClass(std::span<const uint8_t> bytes) { if (bytes.empty()) return std::nullopt; return NodeData{}; }
                std::vector<uint8_t> classToBytes() const { return std::vector<uint8_t>{}; }
            };
            const auto nodeDataId_v = MockData::Helper::make_uuid("019f65ff-1fe9-740f-8326-32b41ff95536");
            const NodeData nodeData;
            nodeEnvDB->nodeSource.createNodeData(query, NodeSource::CreateNodeDataRecord{
                .id = nodeDataId_v,
                .contributorId = nodeContributorId_v,
                .name = "Standard::Type[0]",
                .data = nodeData.classToBytes()
            });

            // --------------------------------------------------------------------------------------------------------------------------------
            // 1.2. Pin Source
            // --------------------------------------------------------------------------------------------------------------------------------
            const auto addPinSourceId_v = MockData::Helper::make_uuid("019f5d11-0c2b-741c-a9ef-b59f7ec0b333");
            nodeEnvDB->pinSource.createPinSource(query, PinSource::CreatePinSourceRecord{
                .id = addPinSourceId_v,
                .globalSourceId = globalSourceId_v,
                .name = "Standard::PinSource"
            });
            const auto addPinContributorId_v = MockData::Helper::make_uuid("019f65ff-1fe9-78ad-a93c-ea9216246666");
            nodeEnvDB->pinSource.createContributor(query, PinSource::CreatePinContributorRecord{
                .id = addPinContributorId_v,
                .sourceId = addPinSourceId_v,
                .name = "Standard::Contributor[0]"
            });

            // Pin Flow
            auto addPinFlowId_v = MockData::Helper::make_uuid("019f65ff-1fe9-7cfb-a993-89e8d828854a");
            nodeEnvDB->pinSource.createFlow(query, PinSource::CreatePinFlowRecord{
                .id = addPinFlowId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::In",
                .degree = 0,
            });
            addPinFlowId_v = MockData::Helper::make_uuid("019f65ff-1fe9-7dae-81cc-2295e8d7bd0b");
            nodeEnvDB->pinSource.createFlow(query, PinSource::CreatePinFlowRecord{
                .id = addPinFlowId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Out",
                .degree = 180,
            });

            // Pin Type
            auto addPinTypeId_v = MockData::Helper::make_uuid("019f65ff-1fe9-7f9d-a47e-1d6cceff0eb0");
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = addPinTypeId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Execution",
                .bit_size = 0
            });
            addPinTypeId_v = MockData::Helper::make_uuid("019f65ff-1fe9-77a7-ba63-dbfd2f8b7a62");
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = addPinTypeId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Object",
                .bit_size = 0
            });

            addPinTypeId_v = MockData::Helper::make_uuid("019f65ff-1fe9-7391-a581-22392c3e6bb8");
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = addPinTypeId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Bool",
                .bit_size = 1
            });
            addPinTypeId_v = MockData::Helper::make_uuid("019f65ff-1fe9-78f3-a827-202e61a9028e");
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = addPinTypeId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Byte",
                .bit_size = 8
            });
            addPinTypeId_v = MockData::Helper::make_uuid("019f65ff-1fe9-782a-84e4-b3e0aa66aaaf");
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = addPinTypeId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Int",
                .bit_size = 32
            });
            addPinTypeId_v = MockData::Helper::make_uuid("019f65ff-1fe9-75ce-8513-2f9c49ccfdd0");
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = addPinTypeId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Int64",
                .bit_size = 64
            });

            addPinTypeId_v = MockData::Helper::make_uuid("019f65ff-1fe9-7fcf-ada1-7c80bb07dc44");
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = addPinTypeId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Float",
                .bit_size = 32
            });
            addPinTypeId_v = MockData::Helper::make_uuid("019f65ff-1fe9-77e4-bf10-7c3bf96f8d75");
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = addPinTypeId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Double",
                .bit_size = 64
            });
            addPinTypeId_v = MockData::Helper::make_uuid("019f65ff-1fe9-726b-9d16-d0996c448a5a");
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = addPinTypeId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Vector2",
                .bit_size = 128
            });
            addPinTypeId_v = MockData::Helper::make_uuid("019f65ff-1fe9-70a1-aa07-bc78a953737d");
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = addPinTypeId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Vector3",
                .bit_size = 192
            });
            addPinTypeId_v = MockData::Helper::make_uuid("019f65ff-1fe9-7c8d-8603-c504a6eb210d");
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = addPinTypeId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Vector",
                .bit_size = -1
            });

            addPinTypeId_v = MockData::Helper::make_uuid("019f65ff-1fe9-750a-9772-d732c2d1dba4");
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = addPinTypeId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Name",
                .bit_size = -1
            });
            addPinTypeId_v = MockData::Helper::make_uuid("019f65ff-1fe9-7b94-ba7c-2adf429f1b53");
            nodeEnvDB->pinSource.createType(query, PinSource::CreatePinTypeRecord{
                .id = addPinTypeId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::String",
                .bit_size = -1
            });

            // Pin Style
            auto addPinStyleId_v = MockData::Helper::make_uuid("019f65ff-1fe9-72ce-b064-d16e00ca0b4f");
            nodeEnvDB->pinSource.createStyle(query, PinSource::CreatePinStyleRecord{
                .id = addPinStyleId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Execution",
                .color = MockData::Color::Execution,
                .wireThickness = 2,
            });
            addPinStyleId_v = MockData::Helper::make_uuid("019f65ff-1fe9-73e9-83d3-d235e9aa8ba3");
            nodeEnvDB->pinSource.createStyle(query, PinSource::CreatePinStyleRecord{
                .id = addPinStyleId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Class",
                .color = MockData::Color::Class,
                .wireThickness = 2,
            });
            addPinStyleId_v = MockData::Helper::make_uuid("019f65ff-1fe9-72d0-b4c4-1e01db3ec35b");
            nodeEnvDB->pinSource.createStyle(query, PinSource::CreatePinStyleRecord{
                .id = addPinStyleId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Object",
                .color = MockData::Color::Object,
                .wireThickness = 2,
            });

            addPinStyleId_v = MockData::Helper::make_uuid("019f65ff-1fe9-7ca5-a824-5021a987e625");
            nodeEnvDB->pinSource.createStyle(query, PinSource::CreatePinStyleRecord{
                .id = addPinStyleId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Bool",
                .color = MockData::Color::Bool,
                .wireThickness = 2,
            });
            addPinStyleId_v = MockData::Helper::make_uuid("019f65ff-1fe9-7a49-a6bc-c0c0f21091d5");
            nodeEnvDB->pinSource.createStyle(query, PinSource::CreatePinStyleRecord{
                .id = addPinStyleId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Byte",
                .color = MockData::Color::Byte,
                .wireThickness = 2,
            });
            addPinStyleId_v = MockData::Helper::make_uuid("019f65ff-1fe9-7df4-9095-76f514383811");
            nodeEnvDB->pinSource.createStyle(query, PinSource::CreatePinStyleRecord{
                .id = addPinStyleId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Int",
                .color = MockData::Color::Int,
                .wireThickness = 2,
            });
            addPinStyleId_v = MockData::Helper::make_uuid("019f65ff-1fe9-70f3-8742-f51366d1ea06");
            nodeEnvDB->pinSource.createStyle(query, PinSource::CreatePinStyleRecord{
                .id = addPinStyleId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Int64",
                .color = MockData::Color::Int64,
                .wireThickness = 2,
            });

            addPinStyleId_v = MockData::Helper::make_uuid("019f65ff-1fe9-7ee2-bada-895eacc8d346");
            nodeEnvDB->pinSource.createStyle(query, PinSource::CreatePinStyleRecord{
                .id = addPinStyleId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Float",
                .color = MockData::Color::Float,
                .wireThickness = 2,
            });
            addPinStyleId_v = MockData::Helper::make_uuid("019f65ff-1fe9-7e06-b429-e6c87b23e1fd");
            nodeEnvDB->pinSource.createStyle(query, PinSource::CreatePinStyleRecord{
                .id = addPinStyleId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Vector",
                .color = MockData::Color::Vector,
                .wireThickness = 2,
            });

            addPinStyleId_v = MockData::Helper::make_uuid("019f65ff-1fe9-70e6-944e-876ed74d148a");
            nodeEnvDB->pinSource.createStyle(query, PinSource::CreatePinStyleRecord{
                .id = addPinStyleId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::Name",
                .color = MockData::Color::Name,
                .wireThickness = 2,
            });
            addPinStyleId_v = MockData::Helper::make_uuid("019f65ff-1fe9-748b-a0d0-267b6f0dc66f");
            nodeEnvDB->pinSource.createStyle(query, PinSource::CreatePinStyleRecord{
                .id = addPinStyleId_v,
                .contributorId = addPinContributorId_v,
                .name = "Standard::String",
                .color = MockData::Color::String,
                .wireThickness = 2,
            });

            // --------------------------------------------------------------------------------------------------------------------------------
            // 1.3. Widget Source
            // --------------------------------------------------------------------------------------------------------------------------------
            const auto addWidgetSourceId_v = MockData::Helper::make_uuid("019f65ff-1fe9-7b0e-9aff-37b814ea1b7a");
            nodeEnvDB->widgetSource.createWidgetSource(query, WidgetSource::CreateWidgetSourceRecord{
                .id = addWidgetSourceId_v,
                .globalSourceId = globalSourceId_v,
                .name = "Standard::WidgetSource"
            });
            const auto addWidgetContributorId_v = MockData::Helper::make_uuid("019f6602-b785-73e3-96b0-7142abe09074");
            nodeEnvDB->widgetSource.createWidgetContributor(query, WidgetSource::CreateWidgetContributorRecord{
                .id = addWidgetContributorId_v,
                .sourceId = addWidgetSourceId_v,
                .name = "Standard::Contributor[0]"
            });

            // Widget Type
            class WidgetTypeMetadata {
            public:
                static std::optional<WidgetTypeMetadata> bytesToClass(std::span<const uint8_t> bytes) { if (bytes.empty()) return std::nullopt; return WidgetTypeMetadata{}; }
                std::vector<uint8_t> classToBytes() const { return std::vector<uint8_t>{}; }
            };
            const auto addWidgetTypeId_v = MockData::Helper::make_uuid("019f6602-b785-7caa-a507-3a9d3c2d5bf9");
            const WidgetTypeMetadata widgetMetadata; // Consistent lowercase prefix
            nodeEnvDB->widgetSource.createWidgetType(query, WidgetSource::CreateWidgetTypeRecord{
                .id = addWidgetTypeId_v,
                .contributorId = addWidgetContributorId_v,
                .name = "Standard::Type[0]",
                .metadata = widgetMetadata.classToBytes()
            });

            // Widget Data
            class WidgetData {
            public:
                static std::optional<WidgetData> bytesToClass(std::span<const uint8_t> bytes) { if (bytes.empty()) return std::nullopt; return WidgetData{}; }
                std::vector<uint8_t> classToBytes() const { return std::vector<uint8_t>{}; }
            };
            const auto addWidgetDataId_v = MockData::Helper::make_uuid("019f6602-b785-7b00-bd4e-12654d7c1c25");
            const WidgetData widgetData;
            nodeEnvDB->widgetSource.createWidgetData(query, WidgetSource::CreateWidgetDataRecord{
                .id = addWidgetDataId_v,
                .contributorId = addWidgetContributorId_v,
                .name = "Standard::Data[0]",
                .data = widgetData.classToBytes()
            });

            // --------------------------------------------------------------------------------------------------------------------------------
            // 1.4. Wire Source
            // --------------------------------------------------------------------------------------------------------------------------------
            const auto addWireSourceId_v = MockData::Helper::make_uuid("019f6602-b785-7499-9a30-119af08fa0b5");
            nodeEnvDB->wireSource.createWireSource(query, WireSource::CreateWireSourceRecord{
                .id = addWireSourceId_v,
                .globalSourceId = globalSourceId_v,
                .name = "Standard::WireSource"
            });

            const auto addWireContributorId_v = MockData::Helper::make_uuid("019f6602-b785-7b44-9c33-27f8a3b34df5");
            nodeEnvDB->wireSource.createWireContributor(query, WireSource::CreateWireContributorRecord{
                .id = addWireContributorId_v,
                .sourceId = addWireSourceId_v,
                .name = "Standard::Contributor[0]"
            });

            // Wire Data
            class WireData {
            public:
                static std::optional<WireData> bytesToClass(std::span<const uint8_t> bytes) { if (bytes.empty()) return std::nullopt; return WireData{}; }
                std::vector<uint8_t> classToBytes() const { return std::vector<uint8_t>{}; }
            };
            const WireData wireData;

            const auto addWireDataId_v = MockData::Helper::make_uuid("019f6602-b785-7041-9d5e-a358357e80ad");
            nodeEnvDB->wireSource.createWireData(query, WireSource::CreateWireDataRecord{
                .id = addWireDataId_v,
                .contributorId = addWireContributorId_v,
                .name = "Standard::Data[0]",
                .data = wireData.classToBytes()
            });

            // Wire Style
            class WireStyleMetadata {
            public:
                static std::optional<WireStyleMetadata> bytesToClass(std::span<const uint8_t> bytes) { if (bytes.empty()) return std::nullopt; return WireStyleMetadata{}; }
                std::vector<uint8_t> classToBytes() const { return std::vector<uint8_t>{}; }
            };
            const WireStyleMetadata styleMetadata;

            auto addWireStyleId_v = MockData::Helper::make_uuid("019f6602-b785-7e98-98dc-5fade0921881");
            nodeEnvDB->wireSource.createWireStyle(query, WireSource::CreateWireStyleRecord{
                .id = addWireStyleId_v,
                .name = "Standard::Execution",
                .color = MockData::Color::Execution,
                .wireThickness = 2,
                .metadata = styleMetadata.classToBytes()
            });
            addWireStyleId_v = MockData::Helper::make_uuid("019f6602-b785-7bdc-b78b-a0290117216e");
            nodeEnvDB->wireSource.createWireStyle(query, WireSource::CreateWireStyleRecord{
                .id = addWireStyleId_v,
                .name = "Standard::Class",
                .color = MockData::Color::Class,
                .wireThickness = 2,
                .metadata = styleMetadata.classToBytes()
            });
            addWireStyleId_v = MockData::Helper::make_uuid("019f6602-b785-7ecb-b0e9-b8fc76efab32");
            nodeEnvDB->wireSource.createWireStyle(query, WireSource::CreateWireStyleRecord{
                .id = addWireStyleId_v,
                .name = "Standard::Object",
                .color = MockData::Color::Object,
                .wireThickness = 2,
                .metadata = styleMetadata.classToBytes()
            });
            addWireStyleId_v = MockData::Helper::make_uuid("019f6602-b785-7281-a5e4-2ad08fe20595");
            nodeEnvDB->wireSource.createWireStyle(query, WireSource::CreateWireStyleRecord{
                .id = addWireStyleId_v,
                .name = "Standard::Bool",
                .color = MockData::Color::Bool,
                .wireThickness = 2,
                .metadata = styleMetadata.classToBytes()
            });
            addWireStyleId_v = MockData::Helper::make_uuid("019f6602-b785-7b38-9ae3-05557a73806c");
            nodeEnvDB->wireSource.createWireStyle(query, WireSource::CreateWireStyleRecord{
                .id = addWireStyleId_v,
                .name = "Standard::Byte",
                .color = MockData::Color::Byte,
                .wireThickness = 2,
                .metadata = styleMetadata.classToBytes()
            });
            addWireStyleId_v = MockData::Helper::make_uuid("019f6602-b785-7376-987c-ed1174260371");
            nodeEnvDB->wireSource.createWireStyle(query, WireSource::CreateWireStyleRecord{
                .id = addWireStyleId_v,
                .name = "Standard::Int",
                .color = MockData::Color::Int,
                .wireThickness = 2,
                .metadata = styleMetadata.classToBytes()
            });
            addWireStyleId_v = MockData::Helper::make_uuid("019f6602-b785-7483-a369-f1d18efedb88");
            nodeEnvDB->wireSource.createWireStyle(query, WireSource::CreateWireStyleRecord{
                .id = addWireStyleId_v,
                .name = "Standard::Int64",
                .color = MockData::Color::Int64,
                .wireThickness = 2,
                .metadata = styleMetadata.classToBytes()
            });
            addWireStyleId_v = MockData::Helper::make_uuid("019f6602-b785-7585-bd51-2875eb6d963d");
            nodeEnvDB->wireSource.createWireStyle(query, WireSource::CreateWireStyleRecord{
                .id = addWireStyleId_v,
                .name = "Standard::Float",
                .color = MockData::Color::Float,
                .wireThickness = 2,
                .metadata = styleMetadata.classToBytes()
            });
            addWireStyleId_v = MockData::Helper::make_uuid("019f6602-b785-7380-9f14-e53c523b80a8");
            nodeEnvDB->wireSource.createWireStyle(query, WireSource::CreateWireStyleRecord{
                .id = addWireStyleId_v,
                .name = "Standard::Vector",
                .color = MockData::Color::Vector,
                .wireThickness = 2,
                .metadata = styleMetadata.classToBytes()
            });
            addWireStyleId_v = MockData::Helper::make_uuid("019f6602-b785-7b0a-8174-f79d9f2577da");
            nodeEnvDB->wireSource.createWireStyle(query, WireSource::CreateWireStyleRecord{
                .id = addWireStyleId_v,
                .name = "Standard::Name",
                .color = MockData::Color::Name,
                .wireThickness = 2,
                .metadata = styleMetadata.classToBytes()
            });
            addWireStyleId_v = MockData::Helper::make_uuid("019f6602-b785-7518-bbfa-3eb397a90f63");
            nodeEnvDB->wireSource.createWireStyle(query, WireSource::CreateWireStyleRecord{
                .id = addWireStyleId_v,
                .name = "Standard::String",
                .color = MockData::Color::String,
                .wireThickness = 2,
                .metadata = styleMetadata.classToBytes()
            });

            // --------------------------------------------------------------------------------------------------------------------------------
            // 2.1. Node Core
            // --------------------------------------------------------------------------------------------------------------------------------
            auto addNodeCoreId_v = MockData::Helper::make_uuid("019f7153-ae5d-72cc-b6eb-d2077d0b5d2e");
            nodeEnvDB->node.createNodeCore(query, Node::CreateNodeCoreRecord{
                .id = addNodeCoreId_v,
                .contributorId = nodeContributorId_v,
                .typeId = nodeDataId_v,
                .dataId = nodeDataId_v,
                .name = "test node core",
                .defaultRowNum = 2,
                .defaultColNum = 1,
                .defaultNodeWidth = -1,
                .defaultNodeHeight = -1
                });

            // --------------------------------------------------------------------------------------------------------------------------------
            // 2.2. Pin Core
            // --------------------------------------------------------------------------------------------------------------------------------
            auto addPinCoreId_v = MockData::Helper::make_uuid("019f7153-ae5d-77d0-908c-9f6107544d95");
            nodeEnvDB->pin.createPinCore(query, Pin::CreatePinCoreRecord{
                .id = addPinCoreId_v,
                .contributorId = addPinContributorId_v,
                .flowId = MockData::Helper::make_uuid("019f65ff-1fe9-7dae-81cc-2295e8d7bd0b"), //Standard::Out
                .typeId = MockData::Helper::make_uuid("019f65ff-1fe9-7391-a581-22392c3e6bb8"), //Standard::Bool
                .styleId = MockData::Helper::make_uuid("019f65ff-1fe9-7ca5-a824-5021a987e625") //Standard::Bool
                });

            nodeEnvDB->pin.createAllowFlows(addPinCoreId_v, QList{ MockData::Helper::make_uuid("019f65ff-1fe9-7dae-81cc-2295e8d7bd0b") }); //allow Standard::Out

            // --------------------------------------------------------------------------------------------------------------------------------
            // 2.3. Widget Core
            // --------------------------------------------------------------------------------------------------------------------------------
            auto addWidgetCoreId_v = MockData::Helper::make_uuid("019f7153-ae5d-7cd2-8104-e01838eb2627");
            nodeEnvDB->widget.createWidgetCore(query, Widget::CreateWidgetCoreRecord{
                .id = addWidgetCoreId_v,
                .contributorId = addWidgetContributorId_v,
                .typeId = addWidgetTypeId_v,
                .dataId = addWidgetDataId_v
                });

            // --------------------------------------------------------------------------------------------------------------------------------
            // 2.4.1 Wire Core
            // --------------------------------------------------------------------------------------------------------------------------------
            auto addWireCoreId_v = MockData::Helper::make_uuid("019f71b0-90be-7462-a862-696c23ff6303");
            nodeEnvDB->wire.createWireCore(query, Wire::CreateWireCoreRecord{
                .id = addWireCoreId_v,
                .contributorId = addWireContributorId_v,
                .styleId = MockData::Helper::make_uuid("019f6602-b785-7281-a5e4-2ad08fe20595"), //Standard::Bool
                .dataId = addWireDataId_v,
                .name = "Standard::Bool",
                });

            // --------------------------------------------------------------------------------------------------------------------------------
            // 2.4.2 Wire Temporary
            // --------------------------------------------------------------------------------------------------------------------------------
            auto addWireTemporaryId_v = MockData::Helper::make_uuid("019f7153-ae5d-7142-82b5-05814f7aeb2b");
            nodeEnvDB->wire.createWireTemporary(query, Wire::CreateWireTemporaryRecord{
                .id = addWireTemporaryId_v,
                .coreId = addWireCoreId_v
                });

            return true;
        });
    }

    // --------------------------------------------------------------------------------------------------------------------------------
    // 3.0. Factories forward declaration
    // --------------------------------------------------------------------------------------------------------------------------------
    inline std::optional<muuid::uuid> createPinInstance(QSqlQuery& query, ANodeEnvDB::ANodeEnvDB* nodeEnvDB);
    inline std::optional<muuid::uuid> createWidgetInstance(QSqlQuery& query, ANodeEnvDB::ANodeEnvDB* nodeEnvDB, const ANodeEnvDB::Config::Widget::FactoryWidgetRecord& newWidget);
    inline std::optional<muuid::uuid> createCellInstance(QSqlQuery& query, ANodeEnvDB::ANodeEnvDB* nodeEnvDB, const ANodeEnvDB::Config::Cell::FactoryCellRecord& newCell);

    // --------------------------------------------------------------------------------------------------------------------------------
    // 3.1. Node Factories 
    // --------------------------------------------------------------------------------------------------------------------------------
    inline std::optional<muuid::uuid> createNodeInstance(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, const QPointF pos) {
        using namespace ANodeEnvDB::Config;
        using namespace ANodeEnvDB::Helpers;

        const muuid::uuid nodeId = muuid::uuid::generate_time_based();

        const bool isValid = useTransaction(nodeEnvDB->getPool(), [&](QSqlQuery& query) {

            const auto newPin = createPinInstance(query, nodeEnvDB);
            if (!newPin) return false;

            const auto newWidget = createWidgetInstance(query, nodeEnvDB, ANodeEnvDB::Config::Widget::FactoryWidgetRecord{
                .w = 10,
                .h = 10
                });
            if (!newWidget) return false;

            class NodeState {
            public:
                static std::optional<NodeState> bytesToClass(std::span<const uint8_t> bytes) { if (bytes.empty()) return std::nullopt; return NodeState{}; }
                std::vector<uint8_t> classToBytes() const { return std::vector<uint8_t>{}; }
            };

            const NodeState state;
            const bool isNodeValid = nodeEnvDB->node.createNode(query, Node::CreateNodeRecord{
                .id = nodeId,
                    .coreId = MockData::Helper::make_uuid("019f7153-ae5d-72cc-b6eb-d2077d0b5d2e"),
                    .name = "test Node",
                    .rowNum = 2,
                    .colNum = 1,
                    .pos = pos,
                    .width = -1,
                    .height = -1,
                    .state = state.classToBytes()
            });
            if (!isNodeValid) return false;

            const auto newCell1 = createCellInstance(query, nodeEnvDB, ANodeEnvDB::Config::Cell::FactoryCellRecord{
                .nodeId = nodeId,
                .name = "cell 1",
                .pinTemplateId = std::nullopt,
                .pinInstanceId = *newPin,
                .widgetId = std::nullopt,
                .isOut = false,
                .row = 0,
                .col = 0,
                .rowSpan = 1,
                .colSpan = 1
                });
            if (!newCell1) return false;

            const auto newCell2 = createCellInstance(query, nodeEnvDB, ANodeEnvDB::Config::Cell::FactoryCellRecord{
                .nodeId = nodeId,
                .name = "cell 2",
                .pinTemplateId = std::nullopt,
                .pinInstanceId = std::nullopt,
                .widgetId = *newWidget,
                .isOut = false,
                .row = 1,
                .col = 0,
                .rowSpan = 1,
                .colSpan = 1
                });
            if (!newCell2) return false;

            return true;
            });

        if (!isValid) return std::nullopt;
        return nodeId;
    }

    // --------------------------------------------------------------------------------------------------------------------------------
    // 3.2. Pin Factories
    // --------------------------------------------------------------------------------------------------------------------------------
    inline std::optional<muuid::uuid> createPinInstance(QSqlQuery& query, ANodeEnvDB::ANodeEnvDB* nodeEnvDB) {
        using namespace ANodeEnvDB::Config;
        using namespace ANodeEnvDB::Helpers;

        const muuid::uuid id = muuid::uuid::generate_time_based();
        const bool isValid = nodeEnvDB->pin.createPin(query, Pin::CreatePinRecord{
            .id = id,
            .coreId = MockData::Helper::make_uuid("019f7153-ae5d-77d0-908c-9f6107544d95") // { Standard::Out, Standard::Bool, Standard::Bool }
            });

        if (!isValid) return std::nullopt;
        return id;
    }

    // --------------------------------------------------------------------------------------------------------------------------------
    // 3.3. Widget Factories
    // --------------------------------------------------------------------------------------------------------------------------------
    inline std::optional<muuid::uuid> createWidgetInstance(QSqlQuery& query, ANodeEnvDB::ANodeEnvDB* nodeEnvDB, const ANodeEnvDB::Config::Widget::FactoryWidgetRecord& newWidget) {
        using namespace ANodeEnvDB::Config;
        using namespace ANodeEnvDB::Helpers;

        class WidgetState {
        public:
            static std::optional<WidgetState> bytesToClass(std::span<const uint8_t> bytes) { if (bytes.empty()) return std::nullopt; return WidgetState{}; }
            std::vector<uint8_t> classToBytes() const { return std::vector<uint8_t>{}; }
        };

        const WidgetState state;
        const muuid::uuid id = muuid::uuid::generate_time_based();
        const bool isValid = nodeEnvDB->widget.createWidget(query, Widget::CreateWidgetRecord{
            .id = id,
            .coreId = MockData::Helper::make_uuid("019f7153-ae5d-7cd2-8104-e01838eb2627"),
            .state = state.classToBytes(),
            .w = newWidget.w,
            .h = newWidget.h,
            });

        if (!isValid) return std::nullopt;
        return id;
    }

    // --------------------------------------------------------------------------------------------------------------------------------
    // 3.4. Cell Factories
    // --------------------------------------------------------------------------------------------------------------------------------
    inline std::optional<muuid::uuid> createCellInstance(QSqlQuery& query, ANodeEnvDB::ANodeEnvDB* nodeEnvDB, const ANodeEnvDB::Config::Cell::FactoryCellRecord& newCell) {
        using namespace ANodeEnvDB::Config;
        using namespace ANodeEnvDB::Helpers;

        const muuid::uuid id = muuid::uuid::generate_time_based();
        const bool isValid = nodeEnvDB->cell.createCell(query, Cell::CreateCellRecord{
            .id = id,
            .nodeId = newCell.nodeId,
            .name = newCell.name,
            .pinTemplateId = newCell.pinTemplateId,
            .pinInstanceId = newCell.pinInstanceId,
            .widgetId = newCell.widgetId,
            .isOut = newCell.isOut,
            .row = newCell.row,
            .col = newCell.col,
            .rowSpan = newCell.rowSpan,
            .colSpan = newCell.colSpan
            });

        if (!isValid) return std::nullopt;
        return id;
    }

    // --------------------------------------------------------------------------------------------------------------------------------
    // 3.5. Wire Factories
    // --------------------------------------------------------------------------------------------------------------------------------
    inline std::optional<muuid::uuid> createWireInstance(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, const ANodeEnvDB::Config::Wire::FactoryWireRecord& newWire) {
        using namespace ANodeEnvDB::Config;
        using namespace ANodeEnvDB::Helpers;

        const muuid::uuid id = muuid::uuid::generate_time_based();

        const bool isValid = useTransaction(nodeEnvDB->getPool(), [&](QSqlQuery& query) {

            class WireState {
            public:
                static std::optional<WireState> bytesToClass(std::span<const uint8_t> bytes) { if (bytes.empty()) return std::nullopt; return WireState{}; }
                std::vector<uint8_t> classToBytes() const { return std::vector<uint8_t>{}; }
            };

            const WireState state;
            return nodeEnvDB->wire.createWire(query, Wire::CreateWireRecord{
                .id = id,
                .coreId = MockData::Helper::make_uuid("019f71b0-90be-7462-a862-696c23ff6303"),
                .originId = newWire.originId,
                .targetId = newWire.targetId,
                .originHintPos = newWire.originHintPos,
                .targetHintPos = newWire.targetHintPos,
                .state = state.classToBytes()
                });
        });

        if (!isValid) return std::nullopt;
        return id;
    }
}