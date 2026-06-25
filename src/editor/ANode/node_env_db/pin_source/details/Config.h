#pragma once

namespace NDPinSourceDetails::Config {

	// 0. Source
	struct Source {
		muuid::uuid id;
		QString name;
	};
	struct SourceInfo {
		QString name;
	};
	struct SourceUpdate {
		std::optional<QString> name;
	};

	// 1. Flow
	struct Flow {
		muuid::uuid id;
		muuid::uuid sourceId;
		QString name;
        qreal degree;
	};
	struct FlowInfo {
		QString name;
		qreal degree = 0.0;
	};
	struct FlowUpdate {
		std::optional<QString> name;
		std::optional<qreal> degree = 0.0;
	};

	// 2. Type
	struct Type {
		muuid::uuid id;
		muuid::uuid sourceId;
		QString name;
        int bit_size;
	};
	struct TypeInfo {
		QString name;
		int bit_size = 0;
	};
	struct TypeUpdate {
		std::optional<QString> name;
		std::optional<int> bit_size;
	};

	// 3. Style
	struct Style {
		muuid::uuid id;
		muuid::uuid sourceId;
		QString name;
        QColor color;
        int wire_thickness;
	};
	struct StyleInfo {
		QString name;
		QColor color = Qt::gray;
		int wire_thickness = 2;
	};
	struct StyleUpdate {
		std::optional<QString> name;
		std::optional<QColor> color;
		std::optional<int> wire_thickness;
	};
}