#pragma once

namespace STStreamerDetails::BoundedQueue {

    template<typename T>
    class BoundedQueue {
        std::queue<T> queue;
        mutable std::mutex mutex;
        std::condition_variable cvPush;
        std::condition_variable cvPop;
        size_t capacity;
        std::atomic<bool> cancelled{ false };
        std::atomic<bool> finished{ false };

    public:
        explicit BoundedQueue(size_t capacity = 50) : capacity(capacity) {}

        ~BoundedQueue() {
            cancel();
        }

        bool pushBlocking(T val) {
            std::unique_lock<std::mutex> lock(mutex);
            cvPush.wait(lock, [this]() {
                return queue.size() < capacity || cancelled.load();
            });

            if (cancelled.load()) {
                return false;
            }

            queue.push(std::move(val));
            cvPop.notify_one();
            return true;
        }

        bool tryPop(T& val) {
            std::unique_lock<std::mutex> lock(mutex);
            if (queue.empty() || cancelled.load()) {
                return false;
            }

            val = std::move(queue.front());
            queue.pop();

            cvPush.notify_one();
            return true;
        }

        void finish() {
            {
                std::lock_guard<std::mutex> lock(mutex);
                finished.store(true);
            }
            cvPop.notify_all();
            cvPush.notify_all();
        }

        void cancel() {
            cancelled.store(true);
            cvPush.notify_all();
            cvPop.notify_all();
        }

        void reset() {
            std::lock_guard<std::mutex> lock(mutex);
            std::queue<T> empty;
            std::swap(queue, empty);
            cancelled.store(false);
        }

        bool isCancelled() const { return cancelled.load(); }

        bool isCompleted() const {
            std::lock_guard<std::mutex> lock(mutex);
            return cancelled.load() || (finished.load() && queue.empty());
        }
    };
}