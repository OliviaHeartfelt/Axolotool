/*

#pragma once

#include <QObject>
#include <QtConcurrent/QtConcurrent> // Gives us QtConcurrent::run
#include "ANodeEnvDB.h"
#include "ANodeConsumer.h"

class ChunkManager : public QObject {
    Q_OBJECT

        ANodeEnvDB* m_db;             // Your database instance
    ANodeConsumer* m_consumer;   // Your UI consumer

public:
    explicit ChunkManager(ANodeEnvDB* db, ANodeConsumer* consumer, QObject* parent = nullptr)
        : QObject(parent), m_db(db), m_consumer(consumer) {
    }

    // Called when the viewport needs to load a chunk
    void loadChunk(const muuid::uuid& chunkId) {

        // 1. Prepare consumer & start 60 FPS frame timer on Main UI Thread
        m_consumer->startLoading();

        // 2. Spawn background task using Qt's thread pool!
        QtConcurrent::run([this, chunkId]() {

            // =========================================================
            //  RUNNING ON BACKGROUND WORKER THREAD
            // =========================================================
            // This runs streamChunkToQueue entirely off the main thread.
            // It queries SQLite row by row and pushes to the bounded queue.
            // If the queue fills up (50 items), it automatically pauses here.
            m_db->nodeStreamer.streamChunkToQueue(chunkId, m_consumer->queue());

            });
    }

    // Called when user pans away or zooms rapidly
    void cancelCurrentLoad() {
        // Unblocks background thread instantly if it's waiting on a full queue
        m_consumer->cancel();
    }
};

*/