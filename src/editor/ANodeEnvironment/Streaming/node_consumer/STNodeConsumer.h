#pragma once

#include "../details/BoundedQueue.h"
#include "../node_streamer/STNodeStreamer.h"

namespace STNodeConsumer {

    class ANodeConsumer : public QObject {
        Q_OBJECT

        QGraphicsScene* scene;
        STStreamerDetails::BoundedQueue::BoundedQueue<STNodeStreamer::Config::NodePayload> queue{ 50 };
        QTimer frameTimer;

        static constexpr int64_t MAX_FRAME_BUDGET_MS = 3;

    public:
        explicit ANodeConsumer(QGraphicsScene* _scene, QObject* parent = nullptr) : QObject(parent), scene(_scene) {
            connect(&frameTimer, &QTimer::timeout, this, &ANodeConsumer::processQueue);
        }

        ~ANodeConsumer() override {
            cancel();
        }

        STStreamerDetails::BoundedQueue::BoundedQueue<STNodeStreamer::Config::NodePayload>& queue() {
            return queue;
        }

        void startLoading() {
            cancel();
            queue.reset();
            frameTimer.start(16);
        }

        void cancel() {
            queue.cancel();
            frameTimer.stop();
        }

    private slots:
        void processQueue() {
            if (!scene) return;

            QElapsedTimer timer;
            timer.start();

            STNodeStreamer::Config::NodePayload payload;

            while (queue.tryPop(payload)) {

                //STNodeConsumerDetails::CreateVisualNode::createVisualNode(scene, payload);

                if (timer.elapsed() >= MAX_FRAME_BUDGET_MS) {
                    return;
                }
            }

            if (queue.isCancelled() || queue.isCancelled()) {
                frameTimer.stop();
            }
        }
    };
}