#pragma once

#include "../../NDConcepts.h"

namespace NDWidgetDetails::Config {

	// Widget Core
	struct FullWidgetCoreRecord {
        muuid::uuid id;
		muuid::uuid contributorId;
		std::optional<muuid::uuid> typeId;
        std::optional<muuid::uuid> dataId;
	};
    struct CreateWidgetCoreRecord {
        muuid::uuid id;
        muuid::uuid contributorId;
        std::optional<muuid::uuid> typeId = std::nullopt;
        std::optional<muuid::uuid> dataId = std::nullopt;
    };
    struct UpdateWidgetCoreRecord {
        std::variant<std::monostate, std::optional<muuid::uuid>> contributorId = std::monostate{};
        std::variant<std::monostate, std::optional<muuid::uuid>> typeId =        std::monostate{};
        std::variant<std::monostate, std::optional<muuid::uuid>> dataId =        std::monostate{};
    };

	// Widget
    template<NDConcepts::ByteConvertible State>
    struct FullWidgetRecord {
        muuid::uuid id;
        muuid::uuid coreId;
        std::optional<State> state;
        qreal w;
        qreal h;
    };
    template<NDConcepts::ByteConvertible State>
    struct CreateWidgetRecord {
        muuid::uuid id;
        muuid::uuid coreId;
        std::optional<State> state = std::nullopt;
        std::optional<qreal> w = std::nullopt;
        std::optional<qreal> h = std::nullopt;
    };
    template<NDConcepts::ByteConvertible State>
    struct UpdateWidgetRecord {
        std::variant<std::monostate, std::optional<State>> state = std::monostate{};
        std::variant<std::monostate, std::optional<qreal>> w =     std::monostate{};
        std::variant<std::monostate, std::optional<qreal>> h =     std::monostate{};
    };
}