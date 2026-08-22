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

        std::unique_ptr<STStreamingManager::LoadingNodeStreamer::STLoadingNodeStreamer> m_loadingNodeStreamer;
        std::unique_ptr<STStreamingManager::LoadingWireStreamer::STLoadingWireStreamer> m_loadingWireStreamer;

        std::unique_ptr<STStreamingManager::LoadingNodeConsumer::STLoadingNodeConsumer> m_loadingNodeConsumer;
        std::unique_ptr<STStreamingManager::LoadingWireConsumer::STLoadingWireConsumer> m_loadingWireConsumer;

        std::unique_ptr<STStreamingManager::SaveStreamer::STSavingStreamer> m_savingStremer;
        std::unique_ptr<STStreamingManager::SaveConsumer::STSavingConsumer> m_savingConsumer;

    public:
        explicit ANodeEnvironment(QWidget* parentWidget = nullptr, QObject* parent = nullptr)
            : QObject(parent)
        {
            m_canvas = new VWCanvas::VWCanvas(&m_registry, parentWidget);
            if (!m_canvas) return;

            m_scene = m_canvas->graphicsScene();
            if (!m_scene) return;
        }

        ~ANodeEnvironment() override {
            cancelSaving();
            cancelLoading();

            closeDatabase();
        }

        bool openDatabase(const QString& dbPath, const QString& connectionBaseName = "ANodeEnvDBConn", int poolSize = 4) {
            if (!m_scene) return false;
            closeDatabase();

            m_db = std::make_unique<ANodeEnvDB::ANodeEnvDB>(dbPath, connectionBaseName);

            if (!m_db->open(poolSize)) {
                m_db.reset();
                return false;
            }
            

            m_loadingNodeConsumer = std::make_unique<STStreamingManager::LoadingNodeConsumer::STLoadingNodeConsumer>(m_scene, m_db.get(), &m_registry, this);
            m_loadingWireConsumer = std::make_unique<STStreamingManager::LoadingWireConsumer::STLoadingWireConsumer>(m_scene, m_db.get(), &m_registry, this);
            m_savingConsumer = std::make_unique<STStreamingManager::SaveConsumer::STSavingConsumer>(m_db.get(), this);

            m_loadingNodeStreamer = std::make_unique<STLoadingNodeStreamer::STLoadingNodeStreamer>(m_db.get(), &m_registry);
            m_loadingWireStreamer = std::make_unique<STLoadingWireStreamer::STLoadingWireStreamer>(m_db.get(), &m_registry);
            m_savingStremer = std::make_unique<STStreamingManager::SaveStreamer::STSavingStreamer>(m_db.get(), &m_registry);

            
            if (!m_loadingNodeConsumer ||
                !m_loadingNodeStreamer ||
                !m_loadingWireConsumer ||
                !m_loadingWireStreamer ||
                !m_savingConsumer ||
                !m_savingStremer
            ) return false;

            m_streamingManager = std::make_unique<STStreamingManager::StreamingManager>(
                m_db.get(),
                m_loadingNodeConsumer.get(),
                m_loadingNodeStreamer.get(),
                m_loadingWireConsumer.get(),
                m_loadingWireStreamer.get(),
                m_savingConsumer.get(),
                m_savingStremer.get(),
                this
            );

            load();

            return true;
        }
        void closeDatabase() {
            cancelLoading();

            m_streamingManager.reset();
            m_loadingWireStreamer.reset();
            m_loadingNodeStreamer.reset();

            if (m_db) {
                m_db->close();
                m_db.reset();
            }
        }

        void save() {
            if (m_streamingManager) {
                m_streamingManager->save();
            }
        }
        void cancelSaving() {
            if (m_streamingManager) {
                m_streamingManager->cancelCurrentSave();
            }
        }

        void load(bool clearViewRegisters = true) {
            if (!m_streamingManager) return;

            if (clearViewRegisters) {
                if (!m_scene) return;

                m_registry.nodeView.pinViewRegistry.clear(m_scene);
                m_registry.nodeView.cellViewRegistry.clear(m_scene);
                m_registry.nodeView.nodeViewRegistry.clear(m_scene);
                m_registry.wireView.wireViewRegistry.clear(m_scene);
            }
            m_streamingManager->load();
        }
        void cancelLoading() {
            if (m_streamingManager) {
                m_streamingManager->cancelCurrentLoad();
            }
        }

        bool spawnNode(
            const muuid::uuid& nodeCoreId,
            const bool continueAtFail = false,
            const bool overrideOnCollision = false
        ) {
            if (!m_db || !m_scene || !m_canvas) return false;

            auto* view = m_canvas->graphicsView();
            if (!view) return false;

            auto* viewport = view->viewport();
            if (!viewport) return false;

            auto* newNode = AView::Node::CreateNode::createNewNode(
                m_db.get(),
                &m_registry,
                nullptr,
                nodeCoreId,
                view->mapToScene(viewport->rect().center()), 
                continueAtFail,
                overrideOnCollision
            );
            if (!newNode) return false;

            newNode->setPos(newNode->pos() - newNode->boundingRect().center());

            m_registry.nodeView.nodeViewRegistry.addVisible(newNode->id(), newNode);
            m_scene->addItem(newNode);

            qDebug() << "> Node created! #Nodes:" << m_registry.nodeView.nodeViewRegistry.sizeVisible() - 1 << "->" << m_registry.nodeView.nodeViewRegistry.sizeVisible();
            return true;
        }

        AView::Canvas::VWCanvas* canvas() const { return m_canvas; }
        QGraphicsScene* scene() const { return m_canvas ? m_canvas->graphicsScene() : nullptr; }

        ARegistry::Registry& registry() { return m_registry; }
        const ARegistry::Registry& registry() const { return m_registry; }

        ANodeEnvDB::ANodeEnvDB* db() { return m_db.get(); }
        bool isOpen() const { return m_db && m_db->isOpen(); }
    };
}