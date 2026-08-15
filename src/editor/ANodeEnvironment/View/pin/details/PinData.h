#pragma once

#include "../../drag_drop/VWDragDrop.h"
#include "../../../../Utility/Utility.h"
#include "Concepts.h"

namespace VWPinDetails::PinData {    

    class PinData {
        muuid::uuid m_flow;
        muuid::uuid m_type;
        muuid::uuid m_style;

    public:
        PinData() = default;
        PinData(const muuid::uuid& flowId, const muuid::uuid& typeId, const muuid::uuid& styleId) : m_flow(flowId), m_type(typeId), m_style(styleId) {}

        void flow(const muuid::uuid& newFlow) { m_flow = newFlow; }
        [[nodiscard]] muuid::uuid flow() const { return m_flow; }

        void type(const muuid::uuid& newType) { m_type = newType; }
        [[nodiscard]] muuid::uuid type() const { return m_type; }

        void style(const muuid::uuid& newStyle) { m_style = newStyle; }
        [[nodiscard]] muuid::uuid style() const { return m_style; }

        friend QDataStream& operator<<(QDataStream& out, const PinData& data) {
            Utility::UUID::toDataStream(out, data.m_flow);
            Utility::UUID::toDataStream(out, data.m_type);
            Utility::UUID::toDataStream(out, data.m_style);
            return out;
        }

        friend QDataStream& operator>>(QDataStream& in, PinData& data) {
            const auto flowOpt = Utility::UUID::fromDataStream(in);
            const auto typeOpt = Utility::UUID::fromDataStream(in);
            const auto styleOpt = Utility::UUID::fromDataStream(in);

            if (flowOpt && typeOpt && styleOpt) {
                data.m_flow = *flowOpt;
                data.m_type = *typeOpt;
                data.m_style = *styleOpt;
            }
            return in;
        }

        bool operator==(const PinData&) const = default;
    };

    static_assert(VWDragDrop::Context::ItemData<PinData>);
    static_assert(Concepts::PinDataConcept<PinData>);
}