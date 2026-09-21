#pragma once

#include "../../NDParser.h"
#include "../../NDConcepts.h"

namespace NDNodeDetails::Config {

	// 1. Node Core
	struct FullNodeCoreRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		std::optional<muuid::uuid> typeId;
		std::optional<muuid::uuid> dataId;
		QString name;
		short defaultRowNum;
		short defaultColNum;
		double defaultNodeWidth;
		double defaultNodeHeight;
		std::optional<muuid::uuid> cellVisualFallbackId;
	};
	struct CreateNodeCoreRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		std::optional<muuid::uuid> typeId;
		std::optional<muuid::uuid> dataId;
		QString name;
		short defaultRowNum = 1;
		short defaultColNum = 1;
		double defaultNodeWidth = -1.0;
		double defaultNodeHeight = -1.0;
		std::optional<muuid::uuid> cellVisualFallbackId = std::nullopt;

		static std::optional<CreateNodeCoreRecord> Parse(const QJsonObject& obj) {

			auto optID = NDParser::parse<muuid::uuid>(obj, "id");
			if (!optID) return std::nullopt;

			auto optConcetibutorID = NDParser::parse<muuid::uuid>(obj, "contributorId");
			if (!optConcetibutorID) return std::nullopt;

			auto typeId = NDParser::parse<muuid::uuid>(obj, "typeId", true);
			auto dataId = NDParser::parse<muuid::uuid>(obj, "dataId", true);

			auto optName = NDParser::parse<QString>(obj, "name");
			if (!optName) return std::nullopt;

			auto defaultRowNum = NDParser::parse<short>(obj, "defaultRowNum");
			if (!defaultRowNum) return std::nullopt;

			auto defaultColNum = NDParser::parse<short>(obj, "defaultColNum");
			if (!defaultColNum) return std::nullopt;

			auto defaultNodeWidth = NDParser::parse<double>(obj, "defaultNodeWidth");
			if (!defaultNodeWidth) return std::nullopt;

			auto defaultNodeHeight = NDParser::parse<double>(obj, "defaultNodeHeight");
			if (!defaultNodeHeight) return std::nullopt;

			auto cellVisualFallbackId = NDParser::parse<muuid::uuid>(obj, "cellVisualFallbackId", true);

			return CreateNodeCoreRecord{
				.id = *optID,
				.contributorId = *optConcetibutorID,
				.typeId = typeId,
				.dataId = dataId,
				.name = *optName,
				.defaultRowNum = *defaultRowNum,
				.defaultColNum = *defaultColNum,
				.defaultNodeWidth = *defaultNodeWidth,
				.defaultNodeHeight = *defaultNodeHeight,
				.cellVisualFallbackId = cellVisualFallbackId
			};
		}
	};
	struct UpdateNodeCoreRecord {
		std::optional<muuid::uuid> id            = std::nullopt;
		std::optional<muuid::uuid> contributorId = std::nullopt;
		std::variant<std::monostate, std::optional<muuid::uuid>> typeId = std::monostate{};
		std::variant<std::monostate, std::optional<muuid::uuid>> dataId = std::monostate{};
		std::optional<QString> name             = std::nullopt;
		std::optional<short> defaultRowNum      = std::nullopt;
		std::optional<short> defaultColNum      = std::nullopt;
		std::optional<double> defaultNodeWidth  = std::nullopt;
		std::optional<double> defaultNodeHeight = std::nullopt;
		std::variant<std::monostate, std::optional<muuid::uuid>> cellVisualFallbackId = std::monostate{};
	};

	// 2. Node
	struct CreateNodeRecord {
		muuid::uuid id;
		muuid::uuid coreId;
		QString name;
		short rowNum = 1;
		short colNum = 1;
		QPointF pos;
		std::optional<double> width  = std::nullopt;
		std::optional<double> height = std::nullopt;
		std::optional<std::vector<uint8_t>> state = std::nullopt;

		static std::optional<CreateNodeRecord> Parse(const QJsonObject& obj) {

			auto optID = NDParser::parse<muuid::uuid>(obj, "id");
			if (!optID) return std::nullopt;

			auto coreId = NDParser::parse<muuid::uuid>(obj, "coreId");
			if (!coreId) return std::nullopt;

			auto name = NDParser::parse<QString>(obj, "name");
			if (!name) return std::nullopt;

			auto rowNum = NDParser::parse<short>(obj, "rowNum");
			if (!rowNum) return std::nullopt;

			auto colNum = NDParser::parse<short>(obj, "colNum");
			if (!colNum) return std::nullopt;

			auto x = NDParser::parse<double>(obj, "x");
			if (!x) return std::nullopt;

			auto y = NDParser::parse<double>(obj, "y");
			if (!y) return std::nullopt;

			auto width = NDParser::parse<double>(obj, "width", true);
			auto height = NDParser::parse<double>(obj, "height", true);

			return CreateNodeRecord{
				.id = *optID,
				.coreId = *coreId,
				.name = *name,
				.rowNum = *rowNum,
				.colNum = *colNum,
				.pos = QPointF(*x, *y),
				.width = width,
				.height = height
			};
		}
	};
	struct FactoryNodeRecord {
		short rowNum = 1;
		short colNum = 1;
		QPointF pos;
		std::optional<double> width = std::nullopt;
		std::optional<double> height = std::nullopt;
	};
	struct UpdateNodeRecord {
		std::optional<muuid::uuid> id     = std::nullopt;
		std::optional<muuid::uuid> coreId = std::nullopt;
		std::optional<QString> name   = std::nullopt;
		std::optional<short>   rowNum = std::nullopt;
		std::optional<short>   colNum = std::nullopt;
		std::optional<QPointF> pos    = std::nullopt;
		std::optional<double>  width  = std::nullopt;
		std::optional<double>  height = std::nullopt;
		std::variant<std::monostate, std::optional<std::vector<uint8_t>>> state = std::monostate{};
	};

	struct FullNodeRecord {
		muuid::uuid id;
		muuid::uuid coreId;
		QString name;
		short rowNum;
		short colNum;
		QPointF pos;
		double width;
		double height;
		std::optional<std::vector<uint8_t>> state;

		static CreateNodeRecord toCreate(const FullNodeRecord& fullRecord) {
			return CreateNodeRecord{
				.id = fullRecord.id,
				.coreId = fullRecord.coreId,
				.name = fullRecord.name,
				.rowNum = fullRecord.rowNum,
				.colNum = fullRecord.colNum,
				.pos = fullRecord.pos,
				.width = fullRecord.width,
				.height = fullRecord.height,
				.state = fullRecord.state
			};
		}
		static UpdateNodeRecord toUpdate(const FullNodeRecord& fullRecord) {
			return UpdateNodeRecord{
				.id = std::nullopt,
				.coreId = std::nullopt,
				.name = fullRecord.name,
				.rowNum = fullRecord.rowNum,
				.colNum = fullRecord.colNum,
				.pos = fullRecord.pos,
				.width = fullRecord.width,
				.height = fullRecord.height,
				.state = std::monostate{}
			};
		}
	};
}