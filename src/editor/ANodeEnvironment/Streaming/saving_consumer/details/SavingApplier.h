#pragma once

#include "../../../Storage/ANodeEnvDB.h"
#include "../../saving_streamer/STSavingStreamer.h"

namespace STSavingConsumerDetails::SavingApplier {

    inline bool apply(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, const STSavingStreamer::StreamedItem<ANodeEnvDB::Config::Node::FullNodeRecord>& nodePayload) {
        if (!nodeEnvDB) return false;

        switch (nodePayload.workflow) {
            case STSavingStreamer::DBWorkflow::Create: {
                return nodeEnvDB->node.createNode(query, ANodeEnvDB::Config::Node::FullNodeRecord::toCreate(nodePayload.item));
            }
            case STSavingStreamer::DBWorkflow::Update: {
                return nodeEnvDB->node.updateNode(query, nodePayload.item.id, ANodeEnvDB::Config::Node::FullNodeRecord::toUpdate(nodePayload.item));
            }
            case STSavingStreamer::DBWorkflow::Delete: {
                return nodeEnvDB->node.removeNode(query, nodePayload.item.id);
            }
            case STSavingStreamer::DBWorkflow::Nothing: {
                return true;
            }
        }
        return false;
    }

    inline bool apply(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, const STSavingStreamer::StreamedItem<ANodeEnvDB::Config::Cell::FullCellRecord>& cellPayload) {
        if (!nodeEnvDB) return false;

        switch (cellPayload.workflow) {
            case STSavingStreamer::DBWorkflow::Create: {
                return nodeEnvDB->cell.createCell(query, ANodeEnvDB::Config::Cell::FullCellRecord::toCreate(cellPayload.item));
            }
            case STSavingStreamer::DBWorkflow::Update: {
                return nodeEnvDB->cell.updateLayout(query, cellPayload.item.id, ANodeEnvDB::Config::Cell::FullCellRecord::toUpdate(cellPayload.item));
            }
            case STSavingStreamer::DBWorkflow::Delete: {
                return nodeEnvDB->cell.removeCell(query, cellPayload.item.id);
            }
            case STSavingStreamer::DBWorkflow::Nothing: {
                return true;
            }
        }
        return false;
    }

    inline bool apply(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QSqlQuery& query, const STSavingStreamer::StreamedItem<ANodeEnvDB::Config::Wire::FullWireRecord>& cellPayload) {
        if (!nodeEnvDB) return false;

        switch (cellPayload.workflow) {
            case STSavingStreamer::DBWorkflow::Create: {
                return nodeEnvDB->wire.createWire(query, ANodeEnvDB::Config::Wire::FullWireRecord::toCreate(cellPayload.item));
            }
            case STSavingStreamer::DBWorkflow::Update: {
                return nodeEnvDB->wire.updateWire(query, cellPayload.item.id, ANodeEnvDB::Config::Wire::FullWireRecord::toUpdate(cellPayload.item));
            }
            case STSavingStreamer::DBWorkflow::Delete: {
                return nodeEnvDB->wire.removeWire(query, cellPayload.item.id);
            }
            case STSavingStreamer::DBWorkflow::Nothing: {
                return true;
            }
        }
        return false;
    }
}