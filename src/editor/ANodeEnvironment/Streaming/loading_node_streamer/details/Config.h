#pragma once

#include "../../../Storage/ANodeEnvDB.h"

namespace STLoadingNodeStreamerDetails::Config {

    using namespace ANodeEnvDB::Config;

    struct NodePayload {
        Node::FullNodeRecord node;
        std::vector<Cell::FullCellRecord> nodeCells;
        std::map<muuid::uuid, Widget::FullWidgetRecord> widgets;
    };
}