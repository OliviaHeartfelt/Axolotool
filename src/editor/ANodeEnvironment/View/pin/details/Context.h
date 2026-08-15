#pragma once

#include "../../../Storage/ANodeEnvDB.h"

namespace VWPinDetails::Context {

    struct FactoryData {
        const std::optional<muuid::uuid> flow;
        const std::optional<muuid::uuid> type;
        const std::optional<muuid::uuid> style;
        const QList<muuid::uuid>* allowFlowVec = nullptr;
        const QList<muuid::uuid>* allowTypeVec = nullptr;
    };

    struct PinFlowContext {
		std::string name;
		double degree;
    };
	struct PinTypeContext {
		std::string name;
		int bit_size;
	};
	struct PinStyleContext {
		std::string name;
		std::string color;
		int wireThickness;
	};

    struct PinContext {
        std::optional<PinFlowContext> pinflow = std::nullopt;
        std::optional<PinTypeContext> pinType = std::nullopt;
        std::optional<PinStyleContext> pinStyle = std::nullopt;
    };
}