#pragma once

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
	};
	struct UpdateNodeDataRecord {
		std::optional<muuid::uuid> id =            std::nullopt;
		std::optional<muuid::uuid> contributorId = std::nullopt;
		std::optional<QString> name =              std::nullopt;
		std::variant<std::monostate, std::optional<std::vector<uint8_t>>> data = std::monostate{};
	};
}