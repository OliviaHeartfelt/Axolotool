#pragma once

#include "../../../Registry/ARegistry.h"
#include "../../../Storage/ANodeEnvDB.h"
#include "../../widget/VWWidget.h"

namespace WVCellDetails::CreateWidget {

    inline VWWidget::WidgetWrapper* createWidgetCell(
        ARegistry::Registry& registry,
        CellItem::CellItem* cellItem,
        const muuid::uuid& widgetCoreId,
        const std::optional<muuid::uuid>& widgetId = std::nullopt,
        const std::optional<QString>& text = std::nullopt,
        std::optional<std::vector<uint8_t>> state = std::nullopt
    ) {
        if (!cellItem) return nullptr;

        const auto widgetCore = registry.node.widgetCoreRegistry.at(widgetCoreId);
        if (!widgetCore) return nullptr;

        const auto fn = registry.nodeFunction.widgetFactoryRegistry.at(widgetCore->visualFactoryId);
        if (!fn) return nullptr;

        VWWidget::Context::WidgetContext widgetContext;

        if (widgetCore->typeId) {
            if (const auto registeredTypeOpt = registry.node.widgetTypeRegistry.at(*widgetCore->typeId)) {
                widgetContext.metadata = VWWidget::Context::WidgetContextItem{
                    .name = registeredTypeOpt->name.toStdString(),
                    .value = registeredTypeOpt->metadata,
                };
            }
        }

        if (widgetCore->dataId) {
            if (const auto registeredDataOpt = registry.node.widgetDataRegistry.at(*widgetCore->dataId)) {
                widgetContext.data = VWWidget::Context::WidgetContextItem{
                    .name = registeredDataOpt->name.toStdString(),
                    .value = registeredDataOpt->data,
                };
            }
        }

        if (state) widgetContext.state = std::move(state);

        auto rawWidget = (*fn)(std::move(widgetContext), text);
        if (!rawWidget) return nullptr;

        std::unique_ptr<QGraphicsItem> widgetGuard(rawWidget);

        auto* widgetWrapper = new VWWidget::WidgetWrapper(cellItem, widgetGuard.get(), widgetCoreId, widgetId);

        return widgetWrapper;
    }
}