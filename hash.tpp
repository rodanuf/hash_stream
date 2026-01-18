#include "hash.hpp"
#include <stdexcept>

template <typename t_key, typename t_value>
hash_table<t_key, t_value>::entry::entry(const t_key& key, const t_value& value)
    : m_key(key), m_value(value) {}

template <typename t_key, typename t_value>
hash_table<t_key, t_value>::hash_table(const std::function<size_t(const t_key&)> &hash_func, int capacity)
    : hash_function(hash_func), count(0), capacity(capacity)
{
    if (capacity < 0)
    {
        throw std::invalid_argument("Capacity must be positive");
    }

    buckets.resize(capacity);
}

template <typename t_key, typename t_value>
int hash_table<t_key, t_value>::get_count() const
{
    return count;
}

template <typename t_key, typename t_value>
int hash_table<t_key, t_value>::get_capacity() const
{
    return capacity;
}

template <typename t_key, typename t_value>
const t_value &hash_table<t_key, t_value>::get(const t_key &key) const
{
    size_t index = hash_function(key) % capacity;
    const auto &bucket = buckets[index];
    for (const auto &entry : bucket)
    {
        if (entry.m_key == key)
        {
            return entry.m_value;
        }
    }
}