#pragma once

namespace NDNodeDetails::Config {

	struct FullNodeRecord {
		muuid::uuid id;
		QString title;
		short rowNum;
		short colNum;
		QPointF pos;
		double width;
		double height;
	};
	struct CreateNodeRecord {
		QString title;
		short rowNum;
		short colNum;
		QPointF pos;
		std::optional<double> width = std::nullopt;
		std::optional<double> height = std::nullopt;
	};
	struct UpdateNodeRecord {
		std::optional<QString> title =  std::nullopt;
		std::optional<short>   rowNum = std::nullopt;
		std::optional<short>   colNum = std::nullopt;
		std::optional<QPointF> pos =    std::nullopt;
		std::optional<double>  width =  std::nullopt;
		std::optional<double>  height = std::nullopt;
	};
}