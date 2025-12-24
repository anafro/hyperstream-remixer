#include <functional>
#include <map>
#include <optional>

namespace HyperstreamRemixer::Maps {
template <typename K, typename V>
[[nodiscard]] auto map_get(const std::map<K, V> &map, const K &key) noexcept -> std::optional<V> {
    auto iterator = map.find(key);
    if (iterator == map.end()) {
        return std::nullopt;
    }

    return iterator->second;
}
} // namespace HyperstreamRemixer::Maps
