#pragma once

#include "../../NDParser.h"
#include "../../NDConcepts.h"

namespace NDNodeSourceDetails::Config {

	// 1. Node Source
	struct FullNodeSourceRecord {
		muuid::uuid id;
		std::optional<muuid::uuid> globalSourceId;
		QString name;
	};
	struct CreateNodeSourceRecord {
		muuid::uuid id;
		std::optional<muuid::uuid> globalSourceId = std::nullopt;
		QString name;

		static std::optional<CreateNodeSourceRecord> Parse(const QJsonObject& obj) {

			auto optID = NDParser::parse<muuid::uuid>(obj, "id");
			if (!optID) return std::nullopt;

			auto optGlobalSourceID = NDParser::parse<muuid::uuid>(obj, "globalSourceId", true);

			auto optName = NDParser::parse<QString>(obj, "name");
			if (!optName) return std::nullopt;

			return CreateNodeSourceRecord{
				.id = *optID,
				.globalSourceId = optGlobalSourceID,
				.name = *optName
			};
		}
	};
	struct UpdateNodeSourceRecord {
		std::optional<muuid::uuid> id =             std::nullopt;
		std::optional<muuid::uuid> globalSourceId = std::nullopt;
		std::optional<QString> name =               std::nullopt;
	};

	// 2. Node Contributor
	struct FullNodeContributorRecord {
		muuid::uuid id;
		muuid::uuid sourceId;
		QString name;
	};
	struct CreateNodeContributorRecord {
		muuid::uuid id;
		muuid::uuid sourceId;
		QString name;

		static std::optional<CreateNodeContributorRecord> Parse(const QJsonObject& obj) {

			auto optID = NDParser::parse<muuid::uuid>(obj, "id");
			if (!optID) return std::nullopt;

			auto optSourceID = NDParser::parse<muuid::uuid>(obj, "sourceId");
			if (!optSourceID) return std::nullopt;

			auto optName = NDParser::parse<QString>(obj, "name");
			if (!optName) return std::nullopt;

			return CreateNodeContributorRecord{
				.id = *optID,
				.sourceId = *optSourceID,
				.name = *optName
			};
		}
	};
	struct UpdateNodeContributorRecord {
		std::optional<muuid::uuid> id =       std::nullopt;
		std::optional<muuid::uuid> sourceId = std::nullopt;
		std::optional<QString> name =         std::nullopt;
	};

	// 3. Node Type
	struct FullNodeTypeRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		QString name;
		std::optional<std::vector<uint8_t>> metadata;
	};
	struct CreateNodeTypeRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		QString name;
		std::optional<std::vector<uint8_t>> metadata = std::nullopt;

		static std::optional<CreateNodeTypeRecord> Parse(const QJsonObject& obj) {

			auto optID = NDParser::parse<muuid::uuid>(obj, "id");
			if (!optID) return std::nullopt;

			auto optContributorID = NDParser::parse<muuid::uuid>(obj, "contributorId");
			if (!optContributorID) return std::nullopt;

			auto optName = NDParser::parse<QString>(obj, "name");
			if (!optName) return std::nullopt;

			return CreateNodeTypeRecord{
				.id = *optID,
				.contributorId = *optContributorID,
				.name = *optName
			};
		}
	};
	struct UpdateNodeTypeRecord {
		std::optional<muuid::uuid> id =            std::nullopt;
		std::optional<muuid::uuid> contributorId = std::nullopt;
		std::optional<QString> name =              std::nullopt;
		std::variant<std::monostate, std::optional<std::vector<uint8_t>>> metadata = std::monostate{};
	};

	// 4. Node Data
	struct FullNodeDataRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		QString name;
		std::optional<std::vector<uint8_t>> data;
	};
	struct CreateNodeDataRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		QString name;
		std::optional<std::vector<uint8_t>> data = std::nullopt;

		static std::optional<CreateNodeDataRecord> Parse(const QJsonObject& obj) {

			auto optID = NDParser::parse<muuid::uuid>(obj, "id");
			if (!optID) return std::nullopt;

			auto optContributorID = NDParser::parse<muuid::uuid>(obj, "contributorId");
			if (!optContributorID) return std::nullopt;

			auto optName = NDParser::parse<QString>(obj, "name");
			if (!optName) return std::nullopt;

			return CreateNodeDataRecord{
				.id = *optID,
				.contributorId = *optContributorID,
				.name = *optName
			};
		}
	};
	struct UpdateNodeDataRecord {
		std::optional<muuid::uuid> id =            std::nullopt;
		std::optional<muuid::uuid> contributorId = std::nullopt;
		std::optional<QString> name =              std::nullopt;
		std::variant<std::monostate, std::optional<std::vector<uint8_t>>> data = std::monostate{};
	};
}