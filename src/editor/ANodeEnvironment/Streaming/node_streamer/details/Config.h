#pragma once

#include "../../../Storage/ANodeEnvDB.h"

namespace STNodeStreamerDetails::Config {    

    using namespace ANodeEnvDB::Config;

    struct NodePayload {
        Node::FullNodeRecord node;
        std::optional<Node::FullNodeCoreRecord> nodeCore;
        std::optional<NodeSource::FullNodeTypeRecord>  nodeType = std::nullopt;
        std::optional<NodeSource::FullNodeDataRecord>  nodeData = std::nullopt;

        std::vector<Cell::FullCellRecord> nodeCells = {};

        std::map<muuid::uuid, Pin::FullPinRecord>     pins = {};
        std::map<muuid::uuid, Pin::FullPinCoreRecord> pinCores = {};
        std::map<muuid::uuid, PinSource::FullPinFlowRecord> pinFlows = {};
        std::map<muuid::uuid, PinSource::FullPinTypeRecord> pinTypes = {};
        std::map<muuid::uuid, PinSource::FullPinStyleRecord> pinStyles = {};

        std::map<muuid::uuid, Widget::FactoryWidgetRecord>  widgets = {};
        std::map<muuid::uuid, Widget::FullWidgetCoreRecord> widgetCores = {};
        std::map<muuid::uuid, WidgetSource::FullWidgetTypeRecord> widgetTypes = {};
        std::map<muuid::uuid, WidgetSource::FullWidgetDataRecord> widgetData = {};
    };

    struct StreamCache {
        std::unordered_map<muuid::uuid, Node::FullNodeCoreRecord> nodeCores;
        std::unordered_map<muuid::uuid, NodeSource::FullNodeTypeRecord>  nodeTypes;
        std::unordered_map<muuid::uuid, NodeSource::FullNodeDataRecord>  nodeData;

        std::unordered_map<muuid::uuid, Pin::FullPinCoreRecord> pinCores;
        std::unordered_map<muuid::uuid, PinSource::FullPinFlowRecord>  pinFlows;
        std::unordered_map<muuid::uuid, PinSource::FullPinTypeRecord>  pinTypes;
        std::unordered_map<muuid::uuid, PinSource::FullPinStyleRecord> pinStyles;

        std::unordered_map<muuid::uuid, Widget::FullWidgetCoreRecord> widgetCores;
        std::unordered_map<muuid::uuid, WidgetSource::FullWidgetTypeRecord> widgetTypes;
        std::unordered_map<muuid::uuid, WidgetSource::FullWidgetDataRecord> widgetData;
    };
}