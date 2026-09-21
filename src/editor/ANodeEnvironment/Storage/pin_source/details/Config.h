#pragma once

#include "../../NDParser.h"

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

		static std::optional<CreatePinSourceRecord> Parse(const QJsonObject& obj) {

			auto optID = NDParser::parse<muuid::uuid>(obj, "id");
			if (!optID) return std::nullopt;

			auto globalSourceId = NDParser::parse<muuid::uuid>(obj, "globalSourceId", true);

			auto name = NDParser::parse<QString>(obj, "name");
			if (!name) return std::nullopt;

			return CreatePinSourceRecord{
				.id = *optID,
				.globalSourceId = globalSourceId,
				.name = *name
			};
		}
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

		static std::optional<CreatePinContributorRecord> Parse(const QJsonObject& obj) {

			auto optID = NDParser::parse<muuid::uuid>(obj, "id");
			if (!optID) return std::nullopt;

			auto sourceId = NDParser::parse<muuid::uuid>(obj, "sourceId");
			if (!sourceId) return std::nullopt;

			auto name = NDParser::parse<QString>(obj, "name");
			if (!name) return std::nullopt;

			return CreatePinContributorRecord{
				.id = *optID,
				.sourceId = *sourceId,
				.name = *name
			};
		}
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

		static std::optional<CreatePinFlowRecord> Parse(const QJsonObject& obj) {

			auto optID = NDParser::parse<muuid::uuid>(obj, "id");
			if (!optID) return std::nullopt;

			auto contributorId = NDParser::parse<muuid::uuid>(obj, "contributorId");
			if (!contributorId) return std::nullopt;

			auto name = NDParser::parse<QString>(obj, "name");
			if (!name) return std::nullopt;

			auto degree = NDParser::parse<double>(obj, "degree");
			if (!degree) return std::nullopt;

			return CreatePinFlowRecord{
				.id = *optID,
				.contributorId = *contributorId,
				.name = *name,
				.degree = *degree
			};
		}
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

		static std::optional<CreatePinTypeRecord> Parse(const QJsonObject& obj) {

			auto optID = NDParser::parse<muuid::uuid>(obj, "id");
			if (!optID) return std::nullopt;

			auto contributorId = NDParser::parse<muuid::uuid>(obj, "contributorId");
			if (!contributorId) return std::nullopt;

			auto name = NDParser::parse<QString>(obj, "name");
			if (!name) return std::nullopt;

			auto bitSize = NDParser::parse<int>(obj, "bitSize");
			if (!bitSize) return std::nullopt;

			return CreatePinTypeRecord{
				.id = *optID,
				.contributorId = *contributorId,
				.name = *name,
				.bit_size = *bitSize
			};
		}
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

		static std::optional<CreatePinStyleRecord> Parse(const QJsonObject& obj) {

			auto optID = NDParser::parse<muuid::uuid>(obj, "id");
			if (!optID) return std::nullopt;

			auto contributorId = NDParser::parse<muuid::uuid>(obj, "contributorId");
			if (!contributorId) return std::nullopt;

			auto name = NDParser::parse<QString>(obj, "name");
			if (!name) return std::nullopt;

			auto color = NDParser::parse<QColor>(obj, "color");
			if (!color) return std::nullopt;

			auto wireThickness = NDParser::parse<int>(obj, "wireThickness");
			if (!wireThickness) return std::nullopt;

			return CreatePinStyleRecord{
				.id = *optID,
				.contributorId = *contributorId,
				.name = *name,
				.color = *color,
				.wireThickness = *wireThickness
			};
		}
	};
	struct UpdatePinStyleRecord {
		std::optional<muuid::uuid> id =            std::nullopt;
		std::optional<muuid::uuid> contributorId = std::nullopt;
		std::optional<QString> name =              std::nullopt;
		std::optional<QColor> color =              std::nullopt;
		std::optional<int> wireThickness =         std::nullopt;
	};
}