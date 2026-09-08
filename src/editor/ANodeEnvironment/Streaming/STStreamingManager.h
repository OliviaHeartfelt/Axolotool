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

        std::unique_ptr<STStreamingManager::LoadingNodeConsumer::STLoadingNodeConsumer> m_loadingNodeConsumer;
        std::unique_ptr<STStreamingManager::LoadingNodeStreamer::STLoadingNodeStreamer> m_loadingNodeStreamer;

        std::unique_ptr<STStreamingManager::LoadingWireConsumer::STLoadingWireConsumer> m_loadingWireConsumer;
        std::unique_ptr<STStreamingManager::LoadingWireStreamer::STLoadingWireStreamer> m_loadingWireStreamer;

        std::unique_ptr<STStreamingManager::SaveConsumer::STSavingConsumer> m_savingConsumer;
        std::unique_ptr<STStreamingManager::SaveStreamer::STSavingStreamer> m_savingStreamer;

        QFuture<void> m_currentLoadingStreamerFuture;
        QFuture<void> m_currentSavingStreamerFuture;

    public:
        explicit StreamingManager(ANodeEnvDB::ANodeEnvDB* db, QGraphicsScene* scene, ARegistry::Registry& m_registry, QObject* parent = nullptr) : m_db(db), QObject(parent) {
            if (!scene || !m_db) return;

            m_loadingNodeConsumer = std::make_unique<STStreamingManager::LoadingNodeConsumer::STLoadingNodeConsumer>(scene, m_db, &m_registry, this);
            m_loadingNodeStreamer = std::make_unique<STLoadingNodeStreamer::STLoadingNodeStreamer>(m_db, &m_registry);
            
            m_loadingWireConsumer = std::make_unique<STStreamingManager::LoadingWireConsumer::STLoadingWireConsumer>(scene, m_db, &m_registry, this);
            m_loadingWireStreamer = std::make_unique<STLoadingWireStreamer::STLoadingWireStreamer>(m_db, &m_registry);

            m_savingConsumer = std::make_unique<STStreamingManager::SaveConsumer::STSavingConsumer>(m_db, this);
            m_savingStreamer = std::make_unique<STStreamingManager::SaveStreamer::STSavingStreamer>(m_db, &m_registry);

            if (m_loadingNodeConsumer) {
                connect(m_loadingNodeConsumer.get(), &STLoadingNodeConsumer::STLoadingNodeConsumer::nodesLoadingFinished,
                    this, &StreamingManager::onNodesPhaseFinished);
            }
            if (m_loadingWireConsumer) {
                connect(m_loadingWireConsumer.get(), &STLoadingWireConsumer::STLoadingWireConsumer::wiresLoadingFinished,
                    this, &StreamingManager::loadFinished);
            }
            if (m_savingConsumer) {
                connect(m_savingConsumer.get(), &STSavingConsumer::STSavingConsumer::saveFinished,
                    this, &StreamingManager::saveFinished);
            }
        }
        ~StreamingManager() override {
            cancelCurrentLoad();
            cancelCurrentSave();

            m_loadingNodeConsumer.reset();
            m_loadingNodeStreamer.reset();
            m_loadingWireConsumer.reset();
            m_loadingWireStreamer.reset();
            m_savingConsumer.reset();
            m_savingStreamer.reset();
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

        bool isValid() const {
            return
                m_loadingNodeConsumer &&
                m_loadingNodeStreamer &&
                m_loadingWireConsumer &&
                m_loadingWireStreamer &&
                m_savingConsumer &&
                m_savingStreamer;
        }
        bool isSaving() const {
            return m_currentSavingStreamerFuture.isRunning();
        }
        bool isLoading() const {
            return m_currentLoadingStreamerFuture.isRunning();
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