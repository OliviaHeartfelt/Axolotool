#pragma once

namespace VWWireDetails::Context {
	
    struct WireStyleContext {
        QString name;
        QColor color;
        int wireThickness;
        std::optional<std::vector<uint8_t>> metadata;
    };
    struct WireDataContext {
        QString name;
        std::optional<std::vector<uint8_t>> data;
    };
	struct WireContext {
        std::optional<WireStyleContext> style;
        std::optional<WireDataContext> data;
        QString name;
	};

    struct FactoryPinData {
        QGraphicsItem* pin{ nullptr };
        std::optional<muuid::uuid> typeId;
        qreal degree{ 0.0 };
    };
}