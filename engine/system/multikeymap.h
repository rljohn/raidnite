#include <unordered_map>
#include <iostream>
#include <type_traits>
#include <cstdint>

template <typename KeyType, typename Value, typename = std::enable_if_t<std::is_integral<KeyType>::value>>
class CombinedKeyMap
{
public:
    using CombinedKeyType = std::conditional_t<(sizeof(KeyType) < 4), uint32_t, uint64_t>;

    void Set(KeyType key1, KeyType key2, const Value& value)
    {
        m_Data[combineKeys(key1, key2)] = value;
    }

    const Value* Get(KeyType key1, KeyType key2) const
    {
        CombinedKeyType combinedKey = combineKeys(key1, key2);
        auto it = m_Data.find(combinedKey);
        return it != m_Data.end() ? &it->second : nullptr;
    }

    bool Exists(KeyType key1, KeyType key2) const
    {
        return m_Data.find(combineKeys(key1, key2)) != m_Data.end();
    }

    void Remove(KeyType key1, KeyType key2)
    {
        m_Data.erase(combineKeys(key1, key2));
    }

private:

    static CombinedKeyType combineKeys(KeyType key1, KeyType key2)
    {
        return (static_cast<CombinedKeyType>(key1) << (sizeof(KeyType) * 8)) | static_cast<CombinedKeyType>(key2);
    }

    std::unordered_map<CombinedKeyType, Value> m_Data;
};
