#pragma once

#include "../../../Storage/ANodeEnvDB.h"

namespace VWNodeDetails::Context {

	struct NodeFactoryData {
		muuid::uuid coreId;
		std::optional<muuid::uuid> id;
		qreal xPos;
		qreal yPos;
		short rowNum;
		short colNum;

		static NodeFactoryData fromNodeRecord(const ANodeEnvDB::Config::Node::FullNodeRecord& nodeRecord) {
			return NodeFactoryData{
				.coreId = nodeRecord.coreId,
				.id = nodeRecord.id,
				.xPos = nodeRecord.pos.x(),
				.yPos = nodeRecord.pos.y(),
				.rowNum = nodeRecord.rowNum, 
				.colNum = nodeRecord.colNum
			};
		}
	};

	struct FactoryData {
		NodeFactoryData node;
		std::vector<ANodeEnvDB::Config::Cell::FullCellRecord> nodeCells;
		std::map<muuid::uuid, ANodeEnvDB::Config::Widget::FullWidgetRecord> widgets;
	};
}