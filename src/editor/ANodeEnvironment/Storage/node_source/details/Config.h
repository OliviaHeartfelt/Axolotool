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
	template<NDConcepts::ByteConvertible Metadata>
	struct FullNodeTypeRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		QString name;
		std::optional<Metadata> metadata;
	};
	template<NDConcepts::ByteConvertible Metadata>
	struct CreateNodeTypeRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		QString name;
		std::optional<Metadata> metadata = std::nullopt;
	};
	template<NDConcepts::ByteConvertible Metadata>
	struct UpdateNodeTypeRecord {
		std::optional<muuid::uuid> id =            std::nullopt;
		std::optional<muuid::uuid> contributorId = std::nullopt;
		std::optional<QString> name =              std::nullopt;
		std::variant<std::monostate, std::optional<Metadata>> metadata = std::monostate{};
	};

	// 4. Node Data
	template<NDConcepts::ByteConvertible Data>
	struct FullNodeDataRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		QString name;
		std::optional<Data> data;
	};
	template<NDConcepts::ByteConvertible Data>
	struct CreateNodeDataRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		QString name;
		std::optional<Data> data = std::nullopt;
	};
	template<NDConcepts::ByteConvertible Data>
	struct UpdateNodeDataRecord {
		std::optional<muuid::uuid> id =            std::nullopt;
		std::optional<muuid::uuid> contributorId = std::nullopt;
		std::optional<QString> name =              std::nullopt;
		std::variant<std::monostate, std::optional<Data>> data = std::monostate{};
	};
}