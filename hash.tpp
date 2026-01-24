#include "hash.hpp"
#include <stdexcept>

template <typename t_key, typename t_value>
hash_table<t_key, t_value>::entry::entry(const t_key& key, const t_value& value)
    : key(key), value(value) {}

template <typename t_key, typename t_value>
hash_table<t_key, t_value>::hash_table(const std::function<int(const t_key&)> &hash_func, int capacity)
    : hash_function(hash_func), count(0), capacity(capacity)
{
    if (capacity < 0)
    {
        throw std::invalid_argument("Capacity must be positive");
    }

    buckets = array_sequence<list_sequence<entry>>(capacity);
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
int hash_table<t_key, t_value>::get_bucket_size(int index) const 
{
    return buckets[index].get_length();
}

template <typename t_key, typename t_value>
int hash_table<t_key, t_value>::calc_value(const t_key &key) const
{
    return hash_function(key);
}

template <typename t_key, typename t_value>
const t_value &hash_table<t_key, t_value>::get(const t_key &key) const
{
    int index = hash_function(key) % capacity;
    const auto &bucket = buckets[index];
    for (int i = 0; i < bucket.get_length(); i++)
    {
        if (bucket.get(i).key == key)
        {
            return bucket.get(i).value;
        }
    }
    throw std::out_of_range("Key not found");
}

template <typename t_key, typename t_value> 
hash_table<t_key, t_value> &hash_table<t_key, t_value>::set(const t_key &key, const t_value &value)
{
    int index = hash_function(key) % capacity;
    auto &bucket = buckets[index];
    if (bucket.get_length() != 0)
    {
        for (auto &entry : bucket)
        {
            if (entry.key == key)
            {
                entry.value = value;
                return *this;
            }
        }
    }
    bucket.append_element(entry(key, value));
    count++;
    resize_if_needed();
    return *this;
}

template <typename t_key, typename t_value>
size_t hash_table<t_key, t_value>::erase(const t_key &key)
{
    int index = hash_function(key) % capacity;
    auto &bucket = buckets[index];
    for (int i = 0; i < bucket.get_length(); i++)
    {
        if (bucket.get(i).key == key)
        {
            bucket.remove_at(i);
            count--;
            return 1;
        }
    }

    return 0;
}

template <typename t_key, typename t_value>
hash_table<t_key, t_value> &hash_table<t_key, t_value>::del(const t_key &key)
{
    erase(key);

    return *this;
}

template <typename t_key, typename t_value>
hash_table<t_key, t_value> &hash_table<t_key, t_value>::set_capacity(int new_capacity)
{
    if (new_capacity > capacity || new_capacity < count)
    {
        rehash(new_capacity);
    }

    capacity = new_capacity;
    return *this;
}

template <typename t_key, typename t_value>
hash_table<t_key, t_value> &hash_table<t_key, t_value>::rehash(int new_capacity)
{
    if (new_capacity < count)
    {
        throw std::invalid_argument("Capacity must be bigger than count");
    }

    if (new_capacity == capacity)
    {
        return *this;
    }

    array_sequence<list_sequence<entry>> old_buckets = buckets;
    buckets.clear();
    buckets = array_sequence<list_sequence<entry>>(new_capacity);
    capacity = new_capacity;

    for (int i = 0; i < (old_buckets.get_length() < new_capacity ? old_buckets.get_length() : new_capacity); i++)
    {
        buckets[i] = old_buckets[i];
    }

    return *this;
}

template <typename t_key, typename t_value>
hash_table<t_key, t_value> &hash_table<t_key, t_value>::resize()
{
    capacity *= 2;    
    array_sequence<list_sequence<entry>> old_buckets = buckets;
    buckets.clear();
    buckets = array_sequence<list_sequence<entry>>(capacity);

    for (int i = 0; i < old_buckets.get_length(); i++)
    {
        buckets[i] = old_buckets[i];
    }

    return *this;
}

template <typename t_key, typename t_value>
hash_table<t_key, t_value> &hash_table<t_key, t_value>::resize_if_needed()
{
    if (count == capacity - 1)
    {
        rehash(capacity * 2);
    }
    return *this;
}

template <typename t_key, typename t_value>
void hash_table<t_key, t_value>::add(const t_key &key, const t_value &value)
{
    this->set(key, value);
}

template <typename t_key, typename t_value>
void hash_table<t_key, t_value>::remove(const t_key &key)
{

    size_t result = erase(key);

    if (result == 0)
    {
        throw std::out_of_range("Key not found");
    }

}

template <typename t_key, typename t_value>
bool hash_table<t_key, t_value>::contains_key(const t_key &key) const
{
    int index = hash_function(key) % capacity;
    const auto &bucket = buckets[index];
    for (int i = 0; i < bucket.get_length(); i++)
    {
        if (bucket.get(i).key == key)
        {
            return true;
        }
    }
    return false;
}

template <typename t_key, typename t_value>
bool hash_table<t_key, t_value>::is_consistent() const
{
    int total = 0;
    for (int i = 0; i < buckets.get_length(); i++)
    {
        total += buckets[i].get_length();
    }
    return total == count;
}

template <typename t_key, typename t_value>
double hash_table<t_key, t_value>::get_load_factor() const
{
    return static_cast<double>(count) / capacity;
}

template <typename t_key, typename t_value>
array_sequence<int> hash_table<t_key, t_value>::get_bucket_distribution() const
{
    array_sequence<int> dist;
    for (int i = 0; i < buckets.get_length(); i++)
    {
        dist.append_element(buckets[i].get_length());
    }
    return dist;
}

template <typename t_key, typename t_value>
i_iterator<t_key> *hash_table<t_key, t_value>::get_keys_iterator() const
{
    i_iterator<t_key> *iterator = new i_iterator<t_key>();
    return iterator;
}