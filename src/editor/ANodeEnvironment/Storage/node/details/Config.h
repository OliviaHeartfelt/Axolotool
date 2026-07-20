#pragma once

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
	};

	// 2. Node
	template<NDConcepts::ByteConvertible State>
	struct FullNodeRecord {
		muuid::uuid id;
		muuid::uuid coreId;
		QString name;
		short rowNum;
		short colNum;
		QPointF pos;
		double width;
		double height;
		std::optional<State> state;
	};
	template<NDConcepts::ByteConvertible State>
	struct CreateNodeRecord {
		muuid::uuid id;
		muuid::uuid coreId;
		QString name;
		short rowNum = 1;
		short colNum = 1;
		QPointF pos;
		std::optional<double> width  = std::nullopt;
		std::optional<double> height = std::nullopt;
		std::optional<State> state   = std::nullopt;
	};
	struct FactoryNodeRecord {
		short rowNum = 1;
		short colNum = 1;
		QPointF pos;
		std::optional<double> width = std::nullopt;
		std::optional<double> height = std::nullopt;
	};
	template<NDConcepts::ByteConvertible State>
	struct UpdateNodeRecord {
		std::optional<muuid::uuid> id     = std::nullopt;
		std::optional<muuid::uuid> coreId = std::nullopt;
		std::optional<QString> name   = std::nullopt;
		std::optional<short>   rowNum = std::nullopt;
		std::optional<short>   colNum = std::nullopt;
		std::optional<QPointF> pos    = std::nullopt;
		std::optional<double>  width  = std::nullopt;
		std::optional<double>  height = std::nullopt;
		std::variant<std::monostate, std::optional<State>> state = std::monostate{};
	};
}