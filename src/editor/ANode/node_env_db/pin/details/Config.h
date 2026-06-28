#pragma once

namespace NDPinDetails::Config {

	struct FullPinRecord {
		muuid::uuid id;
		std::optional<muuid::uuid> flowId;
		std::optional<muuid::uuid> typeId;
		std::optional<muuid::uuid> styleId;
		QList<muuid::uuid> allowFlowList = {};
		QList<muuid::uuid> allowTypeList = {};
	};
	struct PinRecord {
		muuid::uuid id;
		std::optional<muuid::uuid> flowId;
		std::optional<muuid::uuid> typeId;
		std::optional<muuid::uuid> styleId;
	};
	struct CreatePinRecord {
		std::optional<muuid::uuid> flowId;
		std::optional<muuid::uuid> typeId;
		std::optional<muuid::uuid> styleId;
		QList<muuid::uuid> allowFlowList = {};
		QList<muuid::uuid> allowTypeList = {};
	};
}