#pragma once

namespace VWWireDetails::Concepts {

    template <typename T>
    concept WireItem = std::derived_from<T, QGraphicsPathItem> && requires(
        T item,
        const T constItem,
        muuid::uuid uuid,
        bool b
        ) {
        // --- Identity & Core DB Mapping ---
            { constItem.id() } -> std::convertible_to<muuid::uuid>;
            { constItem.coreId() } -> std::same_as<const std::optional<muuid::uuid>&>;
            { item.coreId(uuid) };

            // --- Connection Endpoints ---
            { item.origin() } -> std::convertible_to<QGraphicsItem*>;
            { item.target() } -> std::convertible_to<QGraphicsItem*>;

            // --- Lifetime & Dirty State Tracking ---
            { constItem.isNew() } -> std::convertible_to<bool>;
            { item.setIsNew(b) };
            { constItem.isUpdateNeeded() } -> std::convertible_to<bool>;
            { item.setUpdate(b) };

            // --- Geometry & Visual Updating ---
            { item.updatePath() };
    };
}