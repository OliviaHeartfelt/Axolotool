#pragma once

#include "../details/BoundedQueue.h"

#include "../../Storage/ANodeEnvDB.h"
#include "../../Registry/ARegistry.h"
#include "../../View/AView.h"

namespace STSavingStreamer {

    enum class DBWorkflow {
        Nothing,
        Create,
        Update,
        Delete
    };

    template<typename T>
    struct StreamedItem {
        T item;
        DBWorkflow workflow = DBWorkflow::Nothing;
    };

    class STSavingStreamer {
        ANodeEnvDB::ANodeEnvDB* m_nodeEnvDB = nullptr;
        ARegistry::Registry* m_registry = nullptr;

        std::generator<StreamedItem<ANodeEnvDB::Config::Node::FullNodeRecord>> streamNodes() {
            if (!m_registry) co_return;

            auto extractNode = [](QGraphicsItem* itemPtr) -> std::optional<StreamedItem<ANodeEnvDB::Config::Node::FullNodeRecord>> {
                if (!itemPtr) return std::nullopt;

                auto nodeItem = dynamic_cast<AView::Node::NodeItem::Node*>(itemPtr);
                if (!nodeItem) return std::nullopt;

                auto nodeRecordOpt = AView::Node::NodeItem::Node::toRecord(*nodeItem);
                if (!nodeRecordOpt) return std::nullopt;

                auto* body = nodeItem->body.get();

                DBWorkflow workflow = DBWorkflow::Nothing;
                if (nodeItem->isNew()) {
                    workflow = DBWorkflow::Create;
                }
                else if (nodeItem->isUpdateNeeded() || (body && body->isUpdateNeeded())) {
                    workflow = DBWorkflow::Update;
                }

                return StreamedItem<ANodeEnvDB::Config::Node::FullNodeRecord>{
                    .item = std::move(*nodeRecordOpt),
                        .workflow = workflow
                };
            };

            for (const auto& [id, itemPtr] : m_registry->nodeView.nodeViewRegistry.visible().shared_access()) {
                auto streamedItemOpt = extractNode(itemPtr);
                if (!streamedItemOpt || streamedItemOpt->workflow == DBWorkflow::Nothing) continue;

                co_yield std::move(*streamedItemOpt);
            }
            for (const auto& [id, itemPtr] : m_registry->nodeView.nodeViewRegistry.hidden().shared_access()) {
                auto streamedItemOpt = extractNode(itemPtr);
                if (!streamedItemOpt) continue;

                streamedItemOpt->workflow = DBWorkflow::Delete;
                co_yield std::move(*streamedItemOpt);
            }
        }

        std::generator<StreamedItem<ANodeEnvDB::Config::Cell::FullCellRecord>> streamCells() {
            if (!m_registry) co_return;

            auto extractCell = [](QGraphicsItem* itemPtr) -> std::optional<StreamedItem<ANodeEnvDB::Config::Cell::FullCellRecord>> {
                if (!itemPtr) return std::nullopt;

                auto cellItem = dynamic_cast<AView::Cell::CellItem::CellItem*>(itemPtr);
                if (!cellItem) return std::nullopt;

                auto [row, col, rowSpan, colSpan] = cellItem->cellTransforms();

                auto cellRecordOpt = AView::Cell::CellItem::CellItem::toRecord(*cellItem);
                if (!cellRecordOpt) return std::nullopt;

                DBWorkflow workflow = DBWorkflow::Nothing;
                if (cellItem->isNew()) {
                    workflow = DBWorkflow::Create;
                }
                else if (cellItem->isUpdateNeeded()) {
                    workflow = DBWorkflow::Update;
                }

                return StreamedItem<ANodeEnvDB::Config::Cell::FullCellRecord>{
                    .item = std::move(*cellRecordOpt),
                        .workflow = workflow
                };
            };

            for (const auto& [id, itemPtr] : m_registry->nodeView.cellViewRegistry.visible().shared_access()) {
                auto streamedItemOpt = extractCell(itemPtr);
                if (!streamedItemOpt || streamedItemOpt->workflow == DBWorkflow::Nothing) continue;

                co_yield std::move(*streamedItemOpt);
            }
            for (const auto& [id, itemPtr] : m_registry->nodeView.cellViewRegistry.hidden().shared_access()) {
                auto streamedItemOpt = extractCell(itemPtr);
                if (!streamedItemOpt) continue;

                streamedItemOpt->workflow = DBWorkflow::Delete;
                co_yield std::move(*streamedItemOpt);
            }
        }

        std::generator<StreamedItem<ANodeEnvDB::Config::Wire::FullWireRecord>> streamWires() {
            if (!m_registry) co_return;

            for (const auto& [_, item] : m_registry->nodeView.cellViewRegistry.visible()) {
                AView::Cell::CellItem::CellItem* cellPtr = dynamic_cast<AView::Cell::CellItem::CellItem*>(item);
                if (!cellPtr) continue;
            }

            auto extractWire = [](QGraphicsItem* itemPtr) -> std::optional<StreamedItem<ANodeEnvDB::Config::Wire::FullWireRecord>> {
                if (!itemPtr) return std::nullopt;

                auto wireItem = dynamic_cast<AView::Wire::WireItem*>(itemPtr);
                if (!wireItem || !wireItem->coreId()) return std::nullopt;

                auto* originPinPtr = dynamic_cast<AView::Pin::PinItem*>(wireItem->origin());
                auto* targetPinPtr = dynamic_cast<AView::Pin::PinItem*>(wireItem->target());
                if (!originPinPtr || !targetPinPtr) return std::nullopt;

                auto* originCellPtr = dynamic_cast<AView::Cell::CellItem::CellItem*>(originPinPtr->parentItem());
                auto* targetCellPtr = dynamic_cast<AView::Cell::CellItem::CellItem*>(targetPinPtr->parentItem());
                if (!originCellPtr || !targetCellPtr) return std::nullopt;


                DBWorkflow workflow = DBWorkflow::Nothing;
                if (wireItem->isNew()) {
                    workflow = DBWorkflow::Create;
                }
                else if (wireItem->isUpdateNeeded()) {
                    workflow = DBWorkflow::Update;
                }
                
                ANodeEnvDB::Config::Wire::FullWireRecord wireRecord{
                    .id = wireItem->id(),
                    .coreId = *wireItem->coreId(),
                    .originId = originCellPtr->id(),
                    .targetId = targetCellPtr->id(),
                    .originHintPos = originCellPtr->pos(),
                    .targetHintPos = targetCellPtr->pos(),
                    .state = std::nullopt
                };

                return StreamedItem<ANodeEnvDB::Config::Wire::FullWireRecord>{
                    .item = std::move(wireRecord),
                    .workflow = workflow
                };
            };

            for (const auto& [id, itemPtr] : m_registry->wireView.wireViewRegistry.visible().shared_access()) {
                auto wireItem = extractWire(itemPtr);
                if (!wireItem || wireItem->workflow == DBWorkflow::Nothing) continue;

                co_yield std::move(*wireItem);
            }
            for (const auto& [id, itemPtr] : m_registry->wireView.wireViewRegistry.hidden().shared_access()) {
                auto wireItem = extractWire(itemPtr);
                if (!wireItem) continue;

                wireItem->workflow = DBWorkflow::Delete;

                co_yield std::move(*wireItem);
            }
        }

        template<typename Generator, typename QueuePayload>
        bool pushToQueue(Generator&& generator, STStreamerDetails::BoundedQueue::BoundedQueue<QueuePayload>& queue) {
            for (auto&& payload : generator) {
                if (!queue.pushBlocking(std::move(payload))) return false;
            }
            return true;
        }

    public:
        explicit STSavingStreamer(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, ARegistry::Registry* registry) : m_nodeEnvDB(nodeEnvDB), m_registry(registry) {}

        bool streamNodesToQueue(STStreamerDetails::BoundedQueue::BoundedQueue<StreamedItem<ANodeEnvDB::Config::Node::FullNodeRecord>>& queue) {
            return pushToQueue(streamNodes(), queue);
        }
        bool streamCellsToQueue(STStreamerDetails::BoundedQueue::BoundedQueue<StreamedItem<ANodeEnvDB::Config::Cell::FullCellRecord>>& queue) {
            return pushToQueue(streamCells(), queue);
        }
        bool streamWiresToQueue(STStreamerDetails::BoundedQueue::BoundedQueue<StreamedItem<ANodeEnvDB::Config::Wire::FullWireRecord>>& queue) {
            return pushToQueue(streamWires(), queue);
        }
    };
}