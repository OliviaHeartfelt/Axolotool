#pragma once

namespace NDPinDetails::Config {

	struct CompletePinCore {
		muuid::uuid id;
		muuid::uuid contributorId;
		std::optional<muuid::uuid> flowId;
		std::optional<muuid::uuid> typeId;
		std::optional<muuid::uuid> styleId;
		QList<muuid::uuid> allowFlowList = {};
		QList<muuid::uuid> allowTypeList = {};
	};
	struct FullPinCoreRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		std::optional<muuid::uuid> flowId;
		std::optional<muuid::uuid> typeId;
		std::optional<muuid::uuid> styleId;
	};
	struct CreatePinCoreRecord {
		muuid::uuid id;
		muuid::uuid contributorId;
		std::optional<muuid::uuid> flowId;
		std::optional<muuid::uuid> typeId;
		std::optional<muuid::uuid> styleId;
		QList<muuid::uuid> allowFlowList = {};
		QList<muuid::uuid> allowTypeList = {};
	};
	struct UpdatePinCoreRecord {
		std::optional<muuid::uuid> id =            std::nullopt;
		std::optional<muuid::uuid> contributorId = std::nullopt;
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