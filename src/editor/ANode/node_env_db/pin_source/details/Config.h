#pragma once

namespace NDPinSourceDetails::Config {

	// 0. Source
	struct FullPinSourceRecord {
		muuid::uuid id;
		QString name;
	};
	struct CreatePinSourceRecord {
		muuid::uuid id;
		QString name;
	};
	struct UpdatePinSourceRecord {
		std::optional<QString> name;
	};

	struct FullPinContributorRecord {
		muuid::uuid id;
		muuid::uuid sourceId;
		QString name;
	};
	struct CreatePinContributorRecord {
		muuid::uuid id;
		muuid::uuid sourceId;
		QString name;
	};
	struct UpdatePinContributorRecord {
		std::optional<QString> name;
	};

	// 1. Flow
	struct FullPinFlowRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		QString name;
        qreal degree;
	};
	struct CreatePinFlowRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		QString name;
		qreal degree = 0.0;
	};
	struct UpdatePinFlowRecord {
		std::optional<QString> name;
		std::optional<qreal> degree = 0.0;
	};

	// 2. Type
	struct FullPinTypeRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		QString name;
        int bit_size;
	};
	struct CreatePinTypeRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		QString name;
		int bit_size = 0;
	};
	struct UpdatePinTypeRecord {
		std::optional<QString> name;
		std::optional<int> bit_size;
	};

	// 3. Style
	struct FullPinStyleRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		QString name;
        QColor color;
        int wire_thickness;
	};
	struct CreatePinStyleRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		QString name;
		QColor color = Qt::gray;
		int wire_thickness = 2;
	};
	struct UpdatePinStyleRecord {
		std::optional<QString> name;
		std::optional<QColor> color;
		std::optional<int> wire_thickness;
	};
}