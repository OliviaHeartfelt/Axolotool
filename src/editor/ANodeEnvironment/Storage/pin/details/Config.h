#pragma once

#include "../../NDParser.h"

namespace NDPinDetails::Config {

	struct CompletePinCore {
		muuid::uuid id;
		muuid::uuid contributorId;
		muuid::uuid visualFactoryId;
		std::optional<muuid::uuid> flowId;
		std::optional<muuid::uuid> typeId;
		std::optional<muuid::uuid> styleId;
		QList<muuid::uuid> allowFlowList = {};
		QList<muuid::uuid> allowTypeList = {};
	};
	struct FullPinCoreRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		muuid::uuid visualFactoryId;
		std::optional<muuid::uuid> flowId;
		std::optional<muuid::uuid> typeId;
		std::optional<muuid::uuid> styleId;
	};
	struct CreatePinCoreRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		muuid::uuid visualFactoryId;
		std::optional<muuid::uuid> flowId;
		std::optional<muuid::uuid> typeId;
		std::optional<muuid::uuid> styleId;
		QList<muuid::uuid> allowFlowList = {};
		QList<muuid::uuid> allowTypeList = {};

		static std::optional<CreatePinCoreRecord> Parse(const QJsonObject& obj) {

			auto optID = NDParser::parse<muuid::uuid>(obj, "id");
			if (!optID) return std::nullopt;

			auto contributorId = NDParser::parse<muuid::uuid>(obj, "contributorId");
			if (!contributorId) return std::nullopt;

			auto visualFactoryId = NDParser::parse<muuid::uuid>(obj, "visualFactoryId");
			if (!visualFactoryId) return std::nullopt;

			auto flowId = NDParser::parse<muuid::uuid>(obj, "flowId", true);
			auto typeId = NDParser::parse<muuid::uuid>(obj, "typeId", true);
			auto styleId = NDParser::parse<muuid::uuid>(obj, "styleId", true);

			auto allowFlowList = NDParser::parse<QList<muuid::uuid>>(obj, "allowFlowList");
			if (!allowFlowList) return std::nullopt;

			auto allowTypeList = NDParser::parse<QList<muuid::uuid>>(obj, "allowTypeList");
			if (!allowTypeList) return std::nullopt;

			return CreatePinCoreRecord{
				.id = *optID,
				.contributorId = *contributorId,
				.visualFactoryId = *visualFactoryId,
				.flowId = flowId,
				.typeId = typeId,
				.styleId = styleId,
				.allowFlowList = *allowFlowList,
				.allowTypeList = *allowTypeList
			};
		}
	};
	struct UpdatePinCoreRecord {
		std::optional<muuid::uuid> id =              std::nullopt;
		std::optional<muuid::uuid> contributorId =   std::nullopt;
		std::optional<muuid::uuid> visualFactoryId = std::nullopt;
		std::variant<std::monostate, std::optional<muuid::uuid>> flowId = std::monostate{};
		std::variant<std::monostate, std::optional<muuid::uuid>> typeId = std::monostate{};
		std::variant<std::monostate, std::optional<muuid::uuid>> styleId = std::monostate{};
	};

	struct FullPinRecord {
		muuid::uuid id;
		muuid::uuid coreId;
	};
	struct PinRecord {
		muuid::uuid id;
		muuid::uuid coreId;
	};
	struct CreatePinRecord {
		muuid::uuid id;
		muuid::uuid coreId;
	};
}