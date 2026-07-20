#pragma once

namespace NDPinSourceDetails::Config {

	// 0. Source
	struct FullPinSourceRecord {
		muuid::uuid id;
		std::optional<muuid::uuid> globalSourceId;
		QString name;
	};
	struct CreatePinSourceRecord {
		muuid::uuid id;
		std::optional<muuid::uuid> globalSourceId = std::nullopt;
		QString name;
	};
	struct UpdatePinSourceRecord {
		std::optional<muuid::uuid> id =             std::nullopt;
		std::optional<muuid::uuid> globalSourceId = std::nullopt;
		std::optional<QString> name =               std::nullopt;
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
		std::optional<muuid::uuid> id =       std::nullopt;
		std::optional<muuid::uuid> sourceId = std::nullopt;
		std::optional<QString> name =         std::nullopt;
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
		std::optional<muuid::uuid> id =            std::nullopt;
		std::optional<muuid::uuid> contributorId = std::nullopt;
		std::optional<QString> name =              std::nullopt;
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
		std::optional<muuid::uuid> id =            std::nullopt;
		std::optional<muuid::uuid> contributorId = std::nullopt;
		std::optional<QString> name =              std::nullopt;
		std::optional<int> bit_size =              std::nullopt;
	};

	// 3. Style
	struct FullPinStyleRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		QString name;
        QColor color;
        int wireThickness;
	};
	struct CreatePinStyleRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		QString name;
		QColor color = Qt::gray;
		int wireThickness = 2;
	};
	struct UpdatePinStyleRecord {
		std::optional<muuid::uuid> id =            std::nullopt;
		std::optional<muuid::uuid> contributorId = std::nullopt;
		std::optional<QString> name =              std::nullopt;
		std::optional<QColor> color =              std::nullopt;
		std::optional<int> wireThickness =         std::nullopt;
	};
}