#pragma once

namespace STStreamerDetails::BoundedQueue {

    template<typename T>
    class BoundedQueue {
        struct Cell {
            std::atomic<size_t> sequence;
            alignas(T) unsigned char storage[sizeof(T)];

            T* ptr() { return std::launder(reinterpret_cast<T*>(storage)); }
        };

        std::vector<Cell> buffer;
        const size_t capacity;

        alignas(64) std::atomic<size_t> enqueuePos{ 0 };
        alignas(64) std::atomic<size_t> dequeuePos{ 0 };
        alignas(64) std::atomic<size_t> count{ 0 };

        std::atomic<bool> cancelled{ false };
        std::atomic<bool> finished{ false };

        void destroyRemaining() {
            size_t deqPos = dequeuePos.load(std::memory_order_relaxed);
            size_t enqPos = enqueuePos.load(std::memory_order_relaxed);
            for (size_t pos = deqPos; pos != enqPos; ++pos) {
                buffer[pos % capacity].ptr()->~T();
            }
        }

    public:
        explicit BoundedQueue(size_t capacity = 50)
            : buffer(capacity), capacity(capacity)
        {
            for (size_t i = 0; i < capacity; ++i)
                buffer[i].sequence.store(i, std::memory_order_relaxed);
        }

        ~BoundedQueue() {
            cancel();
            destroyRemaining();
        }

        BoundedQueue(const BoundedQueue&) = delete;
        BoundedQueue& operator=(const BoundedQueue&) = delete;

        bool tryPush(T val) {
            size_t pos = enqueuePos.load(std::memory_order_relaxed);
            Cell* cell;
            for (;;) {
                cell = &buffer[pos % capacity];
                size_t seq = cell->sequence.load(std::memory_order_acquire);
                intptr_t diff = static_cast<intptr_t>(seq) - static_cast<intptr_t>(pos);
                if (diff == 0) {
                    if (enqueuePos.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed))
                        break;
                }
                else if (diff < 0) {
                    return false;
                }
                else {
                    pos = enqueuePos.load(std::memory_order_relaxed);
                }
            }

            ::new (static_cast<void*>(cell->storage)) T(std::move(val));
            cell->sequence.store(pos + 1, std::memory_order_release);

            count.fetch_add(1, std::memory_order_relaxed);
            count.notify_one();
            return true;
        }

        bool pushBlocking(T val) {
            for (;;) {
                if (cancelled.load(std::memory_order_acquire))
                    return false;

                if (tryPush(std::move(val)))
                    return true;

                size_t seenCount = count.load(std::memory_order_acquire);
                if (seenCount >= capacity && !cancelled.load(std::memory_order_acquire)) {
                    count.wait(seenCount, std::memory_order_acquire);
                }
            }
        }

        bool tryPop(T& val) {
            size_t pos = dequeuePos.load(std::memory_order_relaxed);
            Cell* cell;
            for (;;) {
                cell = &buffer[pos % capacity];
                size_t seq = cell->sequence.load(std::memory_order_acquire);
                intptr_t diff = static_cast<intptr_t>(seq) - static_cast<intptr_t>(pos + 1);
                if (diff == 0) {
                    if (dequeuePos.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed))
                        break;
                }
                else if (diff < 0) {
                    return false;
                }
                else {
                    pos = dequeuePos.load(std::memory_order_relaxed);
                }
            }

            T* p = cell->ptr();
            val = std::move(*p);
            p->~T();
            cell->sequence.store(pos + capacity, std::memory_order_release);

            count.fetch_sub(1, std::memory_order_relaxed);
            count.notify_one();
            return true;
        }

        void finish() {
            finished.store(true, std::memory_order_release);
            count.notify_all();
        }

        void cancel() {
            cancelled.store(true, std::memory_order_release);
            finished.store(false, std::memory_order_release);
            count.notify_all();
        }

        void reset() {
            destroyRemaining();
            enqueuePos.store(0, std::memory_order_relaxed);
            dequeuePos.store(0, std::memory_order_relaxed);
            count.store(0, std::memory_order_relaxed);
            for (size_t i = 0; i < capacity; ++i)
                buffer[i].sequence.store(i, std::memory_order_relaxed);
            cancelled.store(false, std::memory_order_release);
        }

        bool isCancelled() const { return cancelled.load(std::memory_order_acquire); }

        bool isCompleted() const {
            return cancelled.load(std::memory_order_acquire) ||
                (finished.load(std::memory_order_acquire) &&
                    count.load(std::memory_order_acquire) == 0);
        }
    };
}