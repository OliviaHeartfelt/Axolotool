#pragma once

#include "details/Concepts.h"

namespace RGBaseRegistry {

    template<typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
    class SharedAccess {
        std::shared_lock<std::shared_mutex> m_lock;
        const std::unordered_map<Key, T, Hash, KeyEqual>& m_map;

    public:
        SharedAccess(std::shared_mutex& mutex, const std::unordered_map<Key, T, Hash, KeyEqual>& map) : m_lock(mutex), m_map(map) {}

        auto begin() const { return m_map.begin(); }
        auto end()   const { return m_map.end(); }

        size_t size() const { return m_map.size(); }
    };

    template<typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
    class UniqueAccess {
        std::unique_lock<std::shared_mutex> m_lock;
        std::unordered_map<Key, T, Hash, KeyEqual>& m_map;

    public:
        UniqueAccess(std::shared_mutex& mutex, std::unordered_map<Key, T, Hash, KeyEqual>& map) : m_lock(mutex), m_map(map) {}

        auto begin() { return m_map.begin(); }
        auto end() { return m_map.end(); }

        auto begin() const { return m_map.begin(); }
        auto end()   const { return m_map.end(); }

        size_t size() const { return m_map.size(); }
    };

    template<typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
    class SharedMultimapAccess {
        std::shared_lock<std::shared_mutex> m_lock;
        const std::unordered_multimap<Key, T, Hash, KeyEqual>& m_map;

    public:
        SharedMultimapAccess(std::shared_mutex& mutex, const std::unordered_multimap<Key, T, Hash, KeyEqual>& map) : m_lock(mutex), m_map(map) {}

        auto begin() const { return m_map.begin(); }
        auto end()   const { return m_map.end(); }

        size_t size() const { return m_map.size(); }
    };
    template<typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
    class UniqueMultimapAccess {
        std::unique_lock<std::shared_mutex> m_lock;
        std::unordered_multimap<Key, T, Hash, KeyEqual>& m_map;

    public:
        UniqueMultimapAccess(std::shared_mutex& mutex, std::unordered_multimap<Key, T, Hash, KeyEqual>& map) : m_lock(mutex), m_map(map) {}

        auto begin() { return m_map.begin(); }
        auto end() { return m_map.end(); }

        auto begin() const { return m_map.begin(); }
        auto end()   const { return m_map.end(); }

        size_t size() const { return m_map.size(); }
    };


    template<typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
        requires RGBaseRegistryDetails::Concepts::BaseRegistryConcept<Key, T, Hash, KeyEqual>
    class BaseRegistry {
        mutable std::shared_mutex m_mutex;
        std::unordered_map<Key, T, Hash, KeyEqual> m_registry;
        using const_iterator = std::unordered_map<Key, T, Hash, KeyEqual>::const_iterator;

    public:
        BaseRegistry() = default;

        auto begin() { return m_registry.begin(); }
        auto end() { return m_registry.end(); }

        auto begin()  const { return m_registry.cbegin(); }
        auto end()    const { return m_registry.cend(); }

        auto cbegin() const { return m_registry.cbegin(); }
        auto cend()   const { return m_registry.cend(); }

        bool insert(const Key& id, const T& value) {
            std::unique_lock guard(m_mutex);
            return m_registry.emplace(id, value).second;
        }
        bool insert(const Key& id, T&& value) {
            std::unique_lock guard(m_mutex);
            return m_registry.emplace(id, std::move(value)).second;
        }
        size_t insert(std::vector<std::pair<Key, T>>&& vec) {
            std::unique_lock guard(m_mutex);
            const size_t initialSize = m_registry.size();

            for (auto& pair : vec) {
                m_registry.emplace(std::move(pair.first), std::move(pair.second));
            }
            return m_registry.size() - initialSize;
        }

        bool erase(const Key& id) {
            std::unique_lock guard(m_mutex);
            return m_registry.erase(id) > 0;
        }

        [[nodiscard]] std::optional<T> at(const Key& id) const requires std::is_copy_constructible_v<T> {
            std::shared_lock guard(m_mutex);
            if (const auto it = m_registry.find(id); it != m_registry.end()) {
                return it->second;
            }
            return std::nullopt;
        }

        template<typename Func>
        bool find_and_apply(const Key& id, Func&& func) const {
            std::shared_lock guard(m_mutex);
            if (const auto it = m_registry.find(id); it != m_registry.end()) {
                std::invoke(std::forward<Func>(func), it->second);
                return true;
            }
            return false;
        }

        [[nodiscard]] SharedAccess<Key, T, Hash, KeyEqual> shared_access() const {
            return SharedAccess<Key, T, Hash, KeyEqual>(m_mutex, m_registry);
        }
        [[nodiscard]] UniqueAccess<Key, T, Hash, KeyEqual> unique_access() {
            return UniqueAccess<Key, T, Hash, KeyEqual>(m_mutex, m_registry);
        }

        [[nodiscard]] bool contains(const Key& id) const {
            std::shared_lock guard(m_mutex);
            return m_registry.contains(id);
        }
        [[nodiscard]] size_t size() const {
            std::shared_lock guard(m_mutex);
            return m_registry.size();
        }
        [[nodiscard]] bool empty() const {
            std::shared_lock guard(m_mutex);
            return m_registry.empty();
        }
        void clear() {
            std::unique_lock guard(m_mutex);
            m_registry.clear();
        }
    };

    template<typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
        requires RGBaseRegistryDetails::Concepts::BaseMultimapRegistryConcept<Key, T, Hash, KeyEqual>
    class BaseMultimapRegistry {
        mutable std::shared_mutex m_mutex;
        std::unordered_multimap<Key, T, Hash, KeyEqual> m_registry;

    public:
        BaseMultimapRegistry() = default;

        auto begin() { return m_registry.begin(); }
        auto end() { return m_registry.end(); }

        auto begin()  const { return m_registry.cbegin(); }
        auto end()    const { return m_registry.cend(); }

        auto cbegin() const { return m_registry.cbegin(); }
        auto cend()   const { return m_registry.cend(); }

        void insert(const Key& id, const T& value) {
            std::unique_lock guard(m_mutex);
            m_registry.emplace(id, value);
        }
        void insert(const Key& id, T&& value) {
            std::unique_lock guard(m_mutex);
            m_registry.emplace(id, std::move(value));
        }

        bool erase(const Key& id) {
            std::unique_lock guard(m_mutex);
            return m_registry.erase(id) > 0;
        }

        void at(const Key& id, QList<T>& list) const {
            std::shared_lock guard(m_mutex);

            auto range = m_registry.equal_range(id);
            for (auto it = range.first; it != range.second; ++it) {
                list.push_back(it->second);
            }
        }
        [[nodiscard]] std::vector<T> at(const Key& id) const {
            std::shared_lock guard(m_mutex);

            std::vector<T> vec;
            auto range = m_registry.equal_range(id);
            for (auto it = range.first; it != range.second; ++it) {
                vec.push_back(it->second);
            }
            return vec;
        }

        [[nodiscard]] SharedMultimapAccess<Key, T, Hash, KeyEqual> shared_access() const {
            return SharedMultimapAccess<Key, T, Hash, KeyEqual>(m_mutex, m_registry);
        }
        [[nodiscard]] UniqueMultimapAccess<Key, T, Hash, KeyEqual> unique_access() {
            return UniqueMultimapAccess<Key, T, Hash, KeyEqual>(m_mutex, m_registry);
        }

        [[nodiscard]] bool contains(const Key& id) const {
            std::shared_lock guard(m_mutex);
            return m_registry.contains(id);
        }
        [[nodiscard]] size_t size() const {
            std::shared_lock guard(m_mutex);
            return m_registry.size();
        }
        [[nodiscard]] bool empty() const {
            std::shared_lock guard(m_mutex);
            return m_registry.empty();
        }
        void clear() {
            std::unique_lock guard(m_mutex);
            m_registry.clear();
        }
    };
}