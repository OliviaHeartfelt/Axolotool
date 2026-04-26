module;

#include <QEvent>

#include <map>
#include <vector>
#include <utility>
#include <functional>
#include <ranges>
#include <concepts>
#include <cstdint>
#include <iterator>

export module UIEventDelegates;


export template<typename T>
concept EventType = std::convertible_to<T, QEvent>;

export template<typename EventType>
class EventDelegates {
    using Callback = std::function<void(EventType)>;
    using Key = std::pair<uint8_t, uint8_t>;
    std::multimap<Key, Callback, std::less<>> registry;

    using NodeHandle = decltype(registry)::node_type;

    class Modify {
        EventDelegates* owner = nullptr;
        NodeHandle handle{};

    public:
        Modify(EventDelegates* _owner, uint8_t _event_id, uint8_t _priority) : owner(_owner) {
            if (owner) {
                auto it = registry.find({ _event_id, _priority });
                if (it != registry.end())
                    handle = registry.extract(it);
                else
                    handle = {};
            }
        }

        Modify& id(uint8_t event_id) {
            if (!handle.empty()) {
                handle.key().first = event_id;
            }
            return *this;
        }
        Modify& priority(uint8_t priority) {
            if (!handle.empty()) {
                handle.key().second = priority;
            }
            return *this;
        }
        Modify& callback(Callback fn) {
            if (!handle.empty()) {
                handle.mapped() = std::move(fn);
            }
            return *this;
        }
        void insert() {
            if (owner && !handle.empty()) {
                owner->registry.insert(std::move(handle));
            }
        }
        bool isValid() const { return !handle.empty(); }
    };

public:
    // Basic info
    bool empty() { return registry.empty(); }
    std::size_t size() { return registry.size(); }
    std::size_t count(uint8_t event_id) { 
        if (registry.empty()) return 0;

        auto begin = registry.lower_bound(std::pair{ event_id, 0 });
        if (begin == registry.end()) return 0;
        auto end = registry.upper_bound(std::pair{ event_id, 255 });

        return std::distance(begin, end);
    }
    bool exists(uint8_t event_id, uint8_t priority) {
        if (registry.empty()) return false;
        if (registry.find(std::pair{ event_id, priority }) == registry.end()) return false;
        return true;
    }

    // Memory management
    void clear() { registry.clear(); }

    // Logic
    void insert(uint8_t event_id, uint8_t priority, Callback fn) {
        registry.emplace(Key{event_id, priority}, std::move(fn));
    }
    bool erase(uint8_t event_id) {
        if (registry.empty()) return false;

        auto begin = registry.lower_bound(Key{ event_id, 0 });
        if (begin == registry.end()) return false;

        auto end = registry.upper_bound(Key{ event_id, 255 });

        registry.erase(begin, end);
        return true;
    }
    bool erase(uint8_t event_id, uint8_t priority) {
        if (registry.empty()) return false;

        auto it = registry.find(std::pair{ event_id, priority });
        if (it == registry.end()) return false;

        registry.erase(it);
        return true;
    }
    NodeHandle extract(uint8_t event_id, uint8_t priority) {
        if (registry.empty()) return {};

        auto it = registry.find(std::pair{ event_id, priority });
        return  registry.extract(it);
    }
    std::vector<NodeHandle> extract(uint8_t event_id) {
        if (registry.empty()) return {};

        auto it = registry.lower_bound(std::pair{ event_id, 0 });
        if (it == registry.end() || it->first.first != event_id) return {};
        auto end = registry.upper_bound(std::pair{ event_id, 255 });

        std::vector<NodeHandle> vec{};
        while (it != end) {
            vec.push_back(registry.extract(it++));
        }
        return vec;
    }
    Modify modify(uint8_t event_id, uint8_t priority) {
        return Modify(this, event_id, priority);
    }

    // Dispatcher
    bool invoke(uint8_t event_id, EventType eventData) {
        if (registry.empty()) return false;

        auto it = registry.lower_bound(std::pair{ event_id, 0 });
        if (it == registry.end()) return false;
        auto end = registry.upper_bound(std::pair{ event_id, 255 });

        for (; it != end; ++it) {
            if (eventData->isAccepted()) break;

            it->second(eventData);
        }
        return true;
    }
};

