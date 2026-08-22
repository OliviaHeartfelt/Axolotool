#pragma once

#include "../../Storage/ANodeEnvDB.h"
#include "../details/BoundedQueue.h"
#include "../saving_streamer/STSavingStreamer.h"
#include "details/SavingApplier.h"

namespace STSavingConsumer {

    class STSavingConsumer : public QObject {
        Q_OBJECT

            ANodeEnvDB::ANodeEnvDB* m_nodeEnvDB = nullptr;

        STStreamerDetails::BoundedQueue::BoundedQueue<STSavingStreamer::StreamedItem<ANodeEnvDB::Config::Node::FullNodeRecord>> m_node_queue{ 50 };
        STStreamerDetails::BoundedQueue::BoundedQueue<STSavingStreamer::StreamedItem<ANodeEnvDB::Config::Cell::FullCellRecord>> m_cells_queue{ 50 };
        STStreamerDetails::BoundedQueue::BoundedQueue<STSavingStreamer::StreamedItem<ANodeEnvDB::Config::Wire::FullWireRecord>> m_wire_queue{ 50 };

        QTimer frameTimer;
        static constexpr int64_t MAX_FRAME_BUDGET_MS = 3;

        bool applySave() {
            if (!m_nodeEnvDB) return true;

            QElapsedTimer timer;
            timer.start();

            STSavingStreamer::StreamedItem<ANodeEnvDB::Config::Node::FullNodeRecord> nodePayload;
            STSavingStreamer::StreamedItem<ANodeEnvDB::Config::Cell::FullCellRecord> cellPayload;
            STSavingStreamer::StreamedItem<ANodeEnvDB::Config::Wire::FullWireRecord> wirePayload;

            return NDHelpers::useTransaction(m_nodeEnvDB->getPool(), [&](QSqlQuery& query) -> bool {

                while (m_node_queue.tryPop(nodePayload)) {
                    if (!STSavingConsumerDetails::SavingApplier::apply(m_nodeEnvDB, query, nodePayload)) {
                        return false;
                    }
                    if (timer.elapsed() >= MAX_FRAME_BUDGET_MS) {
                        return true;
                    }
                }

                while (m_cells_queue.tryPop(cellPayload)) {
                    if (!STSavingConsumerDetails::SavingApplier::apply(m_nodeEnvDB, query, cellPayload)) {
                        return false;
                    }
                    if (timer.elapsed() >= MAX_FRAME_BUDGET_MS) {
                        return true;
                    }
                }

                while (m_wire_queue.tryPop(wirePayload)) {
                    if (!STSavingConsumerDetails::SavingApplier::apply(m_nodeEnvDB, query, wirePayload)) {
                        return false;
                    }
                    if (timer.elapsed() >= MAX_FRAME_BUDGET_MS) {
                        return true;
                    }
                }

                return true;
            });
        }

    public:
        explicit STSavingConsumer(ANodeEnvDB::ANodeEnvDB* nodeEnvDB, QObject* parent = nullptr)
            : QObject(parent), m_nodeEnvDB(nodeEnvDB)
        {
            connect(&frameTimer, &QTimer::timeout, this, &STSavingConsumer::processQueue);
        }

        ~STSavingConsumer() override {
            cancel();
        }

        auto& getNodeQueue() { return m_node_queue; }
        auto& getCellsQueue() { return m_cells_queue; }
        auto& getWireQueue() { return m_wire_queue; }

        void startSaving() {
            cancel();
            m_node_queue.reset();
            m_cells_queue.reset();
            m_wire_queue.reset();
            frameTimer.start(16);
        }

        void cancel() {
            m_node_queue.cancel();
            m_cells_queue.cancel();
            m_wire_queue.cancel();
            frameTimer.stop();
        }

    signals:
        void saveFinished(bool success);

    private slots:
        void processQueue() {
            if (!m_nodeEnvDB) return;
            bool success = applySave();

            if (!success) {
                cancel();
                emit saveFinished(false);
                return;
            }

            if (m_node_queue.isCompleted() && m_cells_queue.isCompleted() && m_wire_queue.isCompleted()) {
                frameTimer.stop();
                emit saveFinished(true);
            }
        }
    };
}