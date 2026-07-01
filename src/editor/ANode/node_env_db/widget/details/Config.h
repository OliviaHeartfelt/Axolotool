#pragma once

namespace NDWidgetDetails::Config {

    template<typename T>
    concept ByteConvertible = requires(const T t, QByteArray byteArray) {
        { T::byteArrayToClass(byteArray) } -> std::same_as<std::optional<T>>;
        { t.classToByteArray() } -> std::same_as<QByteArray>;
    };

	// Widget Core
	struct FullWidgetCoreRecord {
        muuid::uuid id;
		muuid::uuid contributorId;
		muuid::uuid typeId;
		muuid::uuid dataId;
	};
    struct CreateWidgetCoreRecord {
        muuid::uuid contributorId;
        std::optional<muuid::uuid> typeId = std::nullopt;
        std::optional<muuid::uuid> dataId = std::nullopt;
    };
    struct updateWidgetCoreRecord {
        std::variant<std::monostate, std::optional<muuid::uuid>> contributorId = std::monostate{};
        std::variant<std::monostate, std::optional<muuid::uuid>> typeId = std::monostate{};
        std::variant<std::monostate, std::optional<muuid::uuid>> dataId = std::monostate{};
    };

	// Widget
    template<NDWidgetDetails::Config::ByteConvertible State>
	struct FullWidgetRecord {
        muuid::uuid id;
		muuid::uuid coreId;
        State state;
        qreal w;
        qreal h;
	};
    template<NDWidgetDetails::Config::ByteConvertible State>
    struct CreateWidgetRecord {
        muuid::uuid coreId;
        std::optional<State> state = std::nullopt;
        std::optional<qreal> w = std::nullopt;
        std::optional<qreal> h = std::nullopt;
    };
    template<NDWidgetDetails::Config::ByteConvertible State>
    struct UpdateWidgetRecord {
        std::variant<std::monostate, std::optional<State>> state = std::monostate{};
        std::variant<std::monostate, std::optional<qreal>> w = std::monostate{};
        std::variant<std::monostate, std::optional<qreal>> h = std::monostate{};
    };
}