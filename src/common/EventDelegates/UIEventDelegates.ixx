module;

#include <vector>
#include <optional>
#include <utility>
#include <functional>
#include <ranges>
#include <concepts>

export module EventDelegates;


template<typename T>
concept range_t = requires(T & t) {
    std::ranges::begin(t);
    std::ranges::end(t);
};

template<typename EventType>
class EventDelegates {
    using Callback = std::function<void(EventType)>;
    std::vector<Callback> delegates;
public:
    // Basic info
    Callback& at(std::size_t pos) { return delegates.at(pos); }
    bool empty() { return delegates.empty(); }
    std::size_t size() { return delegates.size(); }

    // Memory management
    void reserve(std::size_t n) { delegates.reserve(n); }
    void shink_to_fit() { delegates.shrink_to_fit(); }
    void clear() { delegates.clear(); }

    // Logic
    std::optional<std::size_t> insert(std::size_t pos, Callback function) {
        if (pos > delegates.size()) return std::nullopt;
        delegates.insert(delegates.begin() + pos, std::move(function));
        return pos;
    }
    std::optional<std::size_t> erase(std::size_t pos) {
        if (pos >= delegates.size()) return std::nullopt;
        delegates.erase(delegates.begin() + pos);
        return pos;
    }
    bool erase(std::size_t pos_first, std::size_t pos_last) {
        if (pos_first >= delegates.size() || pos_last >= delegates.size() || pos_first > pos_last)
            return false;
        delegates.erase(delegates.begin() + pos_first, delegates.begin() + pos_last);
        return true;
    }
    void push_back(Callback function) {
        delegates.push_back(std::move(function));
    }
    void pop_back() {
        delegates.pop_back();
    }
    bool swap(std::size_t pos_one, std::size_t pos_two) {
        if (pos_one >= delegates.size() || pos_two >= delegates.size())
            return std::nullopt;
        std::swap(delegates[pos_one], delegates[pos_two]);
        return true;
    }

    // Ranges
    template<typename range_t>
    void insert_range(std::size_t pos, range_t&& function_range) {
        if (pos >= delegates.size())
            return false;
        delegates.insert_range(0, function_range);
        return true;
    }
    template<typename range_t>
    void append_range(range_t&& function_range) {
        for (auto& func : function_range) {
            delegates.push_back(std::move(func));
        }
    }

    // Dispatcher
    void invoke(EventType event) {
        for (const auto& callback : delegates) {
            if (callback) callback(event);
        }
    }
    void invoke_all(EventType event) {
        for (const auto& callback : delegates) {
            callback(event);
        }
    }
};