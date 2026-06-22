#pragma once

namespace NDNodeDetails::Config {

	struct Record {
		muuid::uuid id;
		QString title;
		short rowNum;
		short colNum;
		QPointF pos;
		double width;
		double height;
	};
}