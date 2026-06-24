#pragma once

namespace NDPinDetails::Config {

	struct alignas(8) Record {
		muuid::uuid id;
		std::optional<muuid::uuid> flowId;
		std::optional<muuid::uuid> typeId;
		std::optional<muuid::uuid> styleId;
		QList<muuid::uuid> allowFlowList;
		QList<muuid::uuid> allowTypeList;
	};
	struct alignas(8) PinRecord {
		muuid::uuid id;
		std::optional<muuid::uuid> flowId;
		std::optional<muuid::uuid> typeId;
		std::optional<muuid::uuid> styleId;
	};
}