#pragma once

#include "Storage/ANodeEnvDB.h"
#include "Registry/ARegistry.h"
#include "Streaming/STStreamingManager.h"
#include "View/AView.h"

namespace ANodeEnvironment {

    namespace View      { using namespace AView; }
    namespace Storage   { using namespace ANodeEnvDB; }
    namespace Registry  { using namespace ARegistry; }
    namespace Streaming { using namespace STStreamingManager; }

    class ANodeEnvironment : public QObject {
        Q_OBJECT

    private:
        QGraphicsScene* m_scene = nullptr;

        VWCanvas::VWCanvas* m_canvas = nullptr;
        ARegistry::Registry m_registry;
        std::unique_ptr<ANodeEnvDB::ANodeEnvDB> m_db;
        std::unique_ptr<STStreamingManager::StreamingManager> m_streamingManager;

        std::unique_ptr<STStreamingManager::NodeStreamer::STNodeStreamer> m_nodeStreamer;
        std::unique_ptr<STStreamingManager::WireStreamer::STWireStreamer> m_wireStreamer;

        std::unique_ptr<STStreamingManager::NodeConsumer::STNodeConsumer> m_nodeConsumer;
        std::unique_ptr<STStreamingManager::WireConsumer::STWireConsumer> m_wireConsumer;

    public:
        explicit ANodeEnvironment(QWidget* parentWidget = nullptr, QObject* parent = nullptr)
            : QObject(parent)
        {
            m_canvas = new VWCanvas::VWCanvas(parentWidget);

            if (!m_canvas) return;

            m_scene = m_canvas->graphicsScene();
            if (!m_scene) return;

            m_nodeConsumer = std::make_unique<STStreamingManager::NodeConsumer::STNodeConsumer>(m_scene, m_db.get(), &m_registry, this);
            m_wireConsumer = std::make_unique<STStreamingManager::WireConsumer::STWireConsumer>(m_scene, &m_registry, this);
        }

        ~ANodeEnvironment() override {
            closeDatabase();
        }

        bool openDatabase(const QString& dbPath, const QString& connectionBaseName = "ANodeEnvDBConn", int poolSize = 4) {

            closeDatabase();

            m_db = std::make_unique<ANodeEnvDB::ANodeEnvDB>(dbPath, connectionBaseName);

            if (!m_db->open(poolSize)) {
                m_db.reset();
                return false;
            }

            m_nodeStreamer = std::make_unique<STNodeStreamer::STNodeStreamer>(m_db.get(), &m_registry);
            m_wireStreamer = std::make_unique<STWireStreamer::STWireStreamer>(m_db.get(), &m_registry);

            m_streamingManager = std::make_unique<STStreamingManager::StreamingManager>(
                m_db.get(),
                m_nodeConsumer.get(),
                m_nodeStreamer.get(),
                m_wireConsumer.get(),
                m_wireStreamer.get(),
                this
            );

            return true;
        }

        void closeDatabase() {
            cancelLoading();

            m_streamingManager.reset();
            m_wireStreamer.reset();
            m_nodeStreamer.reset();

            if (m_db) {
                m_db->close();
                m_db.reset();
            }
        }

        void loadChunk(const muuid::uuid& chunkId) {
            if (m_streamingManager) {
                m_streamingManager->loadChunk(chunkId);
            }
        }

        void cancelLoading() {
            if (m_streamingManager) {
                m_streamingManager->cancelCurrentLoad();
            }
        }

        void spawnNode(
            const muuid::uuid& nodeCoreId, 
            const QPointF pos,
            const bool continueAtFail = false,
            const bool overrideOnCollision = false
        ) {
            if (!m_db || !m_scene) return;

            auto* newNode = AView::Node::CreateNode::createNewNode(m_db.get(), &m_registry, nullptr, nodeCoreId, pos, continueAtFail, overrideOnCollision);

            if (!newNode) return;

            m_registry.nodeView.nodeViewRegistry.insert(newNode->id(), newNode);
            m_scene->addItem(newNode);

            qDebug() << "node created!";
        }

        AView::Canvas::VWCanvas* canvas() const { return m_canvas; }
        QGraphicsScene* scene() const { return m_canvas ? m_canvas->graphicsScene() : nullptr; }

        ARegistry::Registry& registry() { return m_registry; }
        const ARegistry::Registry& registry() const { return m_registry; }

        ANodeEnvDB::ANodeEnvDB* db() { return m_db.get(); }
        bool isOpen() const { return m_db && m_db->isOpen(); }
    };
}