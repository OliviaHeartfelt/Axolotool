#pragma once

namespace RGBaseRegistry {

    template<typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
    class BaseRegistry {
        mutable std::shared_mutex m_mutex;
        std::unordered_map<Key, T, Hash, KeyEqual> m_registry;
        using const_iterator = std::unordered_map<Key, T, Hash, KeyEqual>::const_iterator;

    public:
        BaseRegistry() = default;

        bool insert(const Key& id, const T& value) {
            std::unique_lock guard(m_mutex);
            return m_registry.emplace(id, value).second;
        }
        bool insert(const Key& id, T&& value) {
            std::unique_lock guard(m_mutex);
            return m_registry.emplace(id, std::move(value)).second;
        }
        std::size_t insert(std::vector<std::pair<Key, T>>&& vec) {
            std::unique_lock guard(m_mutex);
            const std::size_t initialSize = m_registry.size();

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

        [[nodiscard]] bool contains(const Key& id) const {
            std::shared_lock guard(m_mutex);
            return m_registry.contains(id);
        }
        [[nodiscard]] std::size_t size() const {
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