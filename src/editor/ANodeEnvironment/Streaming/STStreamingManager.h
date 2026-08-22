#pragma once

#include "../Storage/ANodeEnvDB.h"

#include "loading_node_consumer/STLoadingNodeConsumer.h"
#include "loading_node_streamer/STLoadingNodeStreamer.h"

#include "loading_wire_consumer/STLoadingWireConsumer.h"
#include "loading_wire_streamer/STLoadingWireStreamer.h"

#include "saving_consumer/STSavingConsumer.h"
#include "saving_streamer/STSavingStreamer.h"

namespace STStreamingManager {

    namespace LoadingNodeConsumer { using namespace STLoadingNodeConsumer; }
    namespace LoadingNodeStreamer { using namespace STLoadingNodeStreamer; }

    namespace LoadingWireConsumer { using namespace STLoadingWireConsumer; }
    namespace LoadingWireStreamer { using namespace STLoadingWireStreamer; }

    namespace SaveConsumer { using namespace STSavingConsumer; }
    namespace SaveStreamer { using namespace STSavingStreamer; }

    class StreamingManager : public QObject {
        Q_OBJECT

            ANodeEnvDB::ANodeEnvDB* m_db = nullptr;

        STLoadingNodeConsumer::STLoadingNodeConsumer* m_loadingNodeConsumer = nullptr;
        STLoadingNodeStreamer::STLoadingNodeStreamer* m_loadingNodeStreamer = nullptr;

        STLoadingWireConsumer::STLoadingWireConsumer* m_loadingWireConsumer = nullptr;
        STLoadingWireStreamer::STLoadingWireStreamer* m_loadingWireStreamer = nullptr;

        STSavingConsumer::STSavingConsumer* m_savingConsumer = nullptr;
        STSavingStreamer::STSavingStreamer* m_savingStreamer = nullptr;

        QFuture<void> m_currentLoadingStreamerFuture;
        QFuture<void> m_currentSavingStreamerFuture;

    public:
        explicit StreamingManager(
            ANodeEnvDB::ANodeEnvDB* db,
            STLoadingNodeConsumer::STLoadingNodeConsumer* loadingNodeConsumer,
            STLoadingNodeStreamer::STLoadingNodeStreamer* loadingNodeStreamer,
            STLoadingWireConsumer::STLoadingWireConsumer* loadingWireConsumer,
            STLoadingWireStreamer::STLoadingWireStreamer* loadingWireStreamer,
            STSavingConsumer::STSavingConsumer* savingConsumer,
            STSavingStreamer::STSavingStreamer* savingStreamer,
            QObject* parent = nullptr
        ) :
            QObject(parent),
            m_db(db),
            m_loadingNodeConsumer(loadingNodeConsumer),
            m_loadingNodeStreamer(loadingNodeStreamer),
            m_loadingWireConsumer(loadingWireConsumer),
            m_loadingWireStreamer(loadingWireStreamer),
            m_savingConsumer(savingConsumer),
            m_savingStreamer(savingStreamer)
        {
            if (m_loadingNodeConsumer) {
                connect(m_loadingNodeConsumer, &STLoadingNodeConsumer::STLoadingNodeConsumer::nodesLoadingFinished,
                    this, &StreamingManager::onNodesPhaseFinished);
            }
            if (m_loadingWireConsumer) {
                connect(m_loadingWireConsumer, &STLoadingWireConsumer::STLoadingWireConsumer::wiresLoadingFinished,
                    this, &StreamingManager::loadFinished);
            }
            if (m_savingConsumer) {
                connect(m_savingConsumer, &STSavingConsumer::STSavingConsumer::saveFinished,
                    this, &StreamingManager::saveFinished);
            }
        }

        ~StreamingManager() override {
            cancelCurrentLoad();
            cancelCurrentSave();
        }

        void load() {
            qDebug() << "load()";
            cancelCurrentLoad();
            if (!m_loadingNodeConsumer || !m_loadingNodeStreamer) return;

            m_loadingNodeConsumer->startLoading();

            m_currentLoadingStreamerFuture = QtConcurrent::run([this]() {
                m_loadingNodeStreamer->streamChunkToQueue(m_loadingNodeConsumer->getQueue());
                m_loadingNodeConsumer->getQueue().finish();
            });
        }
        void cancelCurrentLoad() {
            if (m_loadingNodeConsumer) m_loadingNodeConsumer->cancel();
            if (m_loadingWireConsumer) m_loadingWireConsumer->cancel();

            if (m_currentLoadingStreamerFuture.isRunning()) {
                m_currentLoadingStreamerFuture.waitForFinished();
            }
        }

        void save() {
            qDebug() << "save()";
            cancelCurrentSave();
            if (!m_savingConsumer || !m_savingStreamer) return;

            m_savingConsumer->startSaving();

            m_currentSavingStreamerFuture = QtConcurrent::run([this]() {
                m_savingStreamer->streamNodesToQueue(m_savingConsumer->getNodeQueue());
                m_savingConsumer->getNodeQueue().finish();

                m_savingStreamer->streamCellsToQueue(m_savingConsumer->getCellsQueue());
                m_savingConsumer->getCellsQueue().finish();

                m_savingStreamer->streamWiresToQueue(m_savingConsumer->getWireQueue());
                m_savingConsumer->getWireQueue().finish();
            });
        }
        void cancelCurrentSave() {
            if (m_savingConsumer) m_savingConsumer->cancel();

            if (m_currentSavingStreamerFuture.isRunning()) {
                m_currentSavingStreamerFuture.waitForFinished();
            }
        }

    signals:
        void loadFinished();
        void saveFinished(bool success);

    private slots:
        void onNodesPhaseFinished() {
            if (!m_loadingWireConsumer || !m_loadingWireStreamer) return;

            if (m_currentLoadingStreamerFuture.isRunning()) {
                m_currentLoadingStreamerFuture.waitForFinished();
            }

            m_loadingWireConsumer->startLoading();

            m_currentLoadingStreamerFuture = QtConcurrent::run([this]() {
                m_loadingWireStreamer->streamChunkToQueue(m_loadingWireConsumer->getQueue());
                m_loadingWireConsumer->getQueue().finish();
                });
        }
    };
}