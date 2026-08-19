#pragma once

namespace VWPinDetails::Concepts {

    template<typename T>
    concept PinDataConcept = requires(T t, const muuid::uuid& id) {
        { t.flow(id) } -> std::same_as<void>;
        { t.type(id) } -> std::same_as<void>;
        { t.style(id) } -> std::same_as<void>;

        { t.flow() } -> std::same_as<muuid::uuid>;
        { t.type() } -> std::same_as<muuid::uuid>;
        { t.style() } -> std::same_as<muuid::uuid>;
    };

    template <typename T>
    concept PinAllowSetConcept = requires(T t, const muuid::uuid & id, const std::vector<muuid::uuid>&idVec, const std::function<bool(muuid::uuid)>& pred_fn, bool (*pred)(const muuid::uuid&)) {
        { t.insert(id) } -> std::same_as<bool>;
        { t.insert(idVec) } -> std::same_as<bool>;
        { t.erase(id) } -> std::same_as<bool>;

        { t.erase_if(pred_fn) } -> std::same_as<std::size_t>;
        { t.erase_if(pred) } -> std::same_as<std::size_t>;

        { t.clear() } -> std::same_as<void>;

        { t.contains(id) } -> std::same_as<bool>;
        { t.size() } -> std::same_as<std::size_t>;
        { t.empty() } -> std::same_as<bool>;
    };       

    template<typename T>
    concept PinItemConcept = std::derived_from<T, QGraphicsSvgItem> && requires(
        T t,
        const T c_t,
        const T* cptr_t, 
        const QMarginsF& margins,
        const QString & iconPath
    ) {
        { t.setPadding(margins) } -> std::same_as<void>;
        { t.safeUpdate() }        -> std::same_as<void>;
        //{ t.setSvg(iconPath) }    -> std::same_as<void>;
        { c_t.coreId() }          -> std::same_as<const muuid::uuid&>;

        // Wire Registration
        //{ t.registerWire(wire) }   -> std::same_as<void>;
        //{ t.unregisterWire(wire) } -> std::same_as<void>;


        { t.pinData() } -> PinDataConcept;
            requires std::is_lvalue_reference_v<decltype(t.pinData())>;

        { t.allowFlowSet() } -> PinAllowSetConcept;
            requires std::is_lvalue_reference_v<decltype(t.allowFlowSet())>;

        { t.allowTypeSet() } -> PinAllowSetConcept;
            requires std::is_lvalue_reference_v<decltype(t.allowTypeSet())>;


        c_t.pinData();
        requires (
            PinDataConcept<std::remove_cvref_t<decltype(c_t.pinData())>> &&
            std::is_lvalue_reference_v<decltype(c_t.pinData())> &&
            std::is_const_v<std::remove_reference_t<decltype(c_t.pinData())>>
        );
        
        c_t.allowFlowSet();
        requires (
            PinAllowSetConcept<std::remove_cvref_t<decltype(c_t.allowFlowSet())>> &&
            std::is_lvalue_reference_v<decltype(c_t.allowFlowSet())> && 
            std::is_const_v<std::remove_reference_t<decltype(c_t.allowFlowSet())>>
        );

        c_t.allowTypeSet();
        requires (
            PinAllowSetConcept<std::remove_cvref_t<decltype(c_t.allowTypeSet())>> &&
            std::is_lvalue_reference_v<decltype(c_t.allowTypeSet())> &&
            std::is_const_v<std::remove_reference_t<decltype(c_t.allowTypeSet())>>
        );

        { c_t.isCompatibleSourcePin(cptr_t) } -> std::same_as<bool>;
    };
}