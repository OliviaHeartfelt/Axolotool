#pragma once

namespace WVCellDetails::Concepts {

    template <typename T>
    concept CellItem = std::derived_from<T, QGraphicsRectItem>&& requires(
        T item,
        const T constItem,
        std::optional<muuid::uuid> optUuid,
        std::optional<QString> optString,
        bool b,
        short s
        ) {
        // --- Identity & Ownership ---
            { constItem.id() } -> std::convertible_to<muuid::uuid>;
            { constItem.nodeId() } -> std::same_as<const std::optional<muuid::uuid>>;
            { item.nodeId(optUuid) };

            // --- Lifetime & State Tracking ---
            { constItem.isNew() } -> std::convertible_to<bool>;
            { item.setIsNew(b) };
            { constItem.isUpdateNeeded() } -> std::convertible_to<bool>;
            { item.setUpdate(b) };

            // --- Grid Transform Properties ---
            { constItem.cellTransforms() } -> std::same_as<std::tuple<short, short, short, short>>;
            { item.cellTransforms(s, s, s, s) };

            // --- Metadata & Content Getters/Setters ---
            { constItem.name() } -> std::same_as<std::optional<QString>>;
            { item.name(optString) };
            { constItem.isOut() } -> std::convertible_to<bool>;
            { item.isOut(b) };

            // --- Slot Reference IDs ---
            { constItem.pinTemplateId() } -> std::same_as<std::optional<muuid::uuid>>;
            { item.pinTemplateId(optUuid) };
            { constItem.pinInstanceId() } -> std::same_as<std::optional<muuid::uuid>>;
            { item.pinInstanceId(optUuid) };
            { constItem.widgetId() } -> std::same_as<std::optional<muuid::uuid>>;
            { item.widgetId(optUuid) };

            // --- Validation Logic ---
            { constItem.isContentValid() } -> std::convertible_to<bool>;
            { constItem.isTransformValid() } -> std::convertible_to<bool>;

            // --- DB Record Serialization ---
            { T::toRecord(constItem) } -> std::same_as<std::optional<ANodeEnvDB::Config::Cell::FullCellRecord>>;
    };
}