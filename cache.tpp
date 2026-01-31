#include "cache.hpp"
#include <stdexcept>

template <typename t_key, typename t_value>
cache<t_key, t_value>::cache(int cap, const std::function<int(const t_key&)> &hash_func, const std::string &stream_path)
    : table(hash_func), stream(stream_path), capacity(cap), hit_count(0), miss_count(0)
{
    if (cap <= 0)
    {
        throw std::invalid_argument("Cache capacity must be positive");
    }
}

template <typename t_key, typename t_value>
t_value cache<t_key, t_value>::get(const t_key &key) 
{
    if (table.contains_key(key))
    {
        hit_count++;

        this->update_access_order(key);
        return table.get(key);
    }
    else
    {
        miss_count++;
        t_value value;
        if (this->read_from_stream(key, value))
        {
            table.add(key, value);
            this->update_access_order(key);
            return value;
        }
        else
        {
            throw std::out_of_range("Key not found in cache or backing store");
        }
    }
}

template <typename t_key, typename t_value>
void cache<t_key, t_value>::put(const t_key &key, const t_value &value)
{
    if (table.contains_key(key))
    {
        table.remove(key);
    }
    table.add(key, value);
    update_access_order(key);

    write_to_stream(key, value);
}

template <typename t_key, typename t_value>
void cache<t_key, t_value>::reset_statistics()
{
    hit_count = 0;
    miss_count = 0;
}

template <typename t_key, typename t_value>
int cache<t_key, t_value>::get_hit_count() const
{
    return hit_count;
}

template <typename t_key, typename t_value>
int cache<t_key, t_value>::get_miss_count() const
{
    return miss_count;
}

template <typename t_key, typename t_value>
int cache<t_key, t_value>::get_size() const
{
    return table.get_count();
}

template <typename t_key, typename t_value>
double cache<t_key, t_value>::get_hit_ratio() const
{
    int total = hit_count + miss_count;
    if (total == 0)
        return 0.0;
    return static_cast<double>(hit_count) / total;
}

template <typename t_key, typename t_value>
void cache<t_key, t_value>::update_access_order(const t_key &key)
{
    for (int i = 0; i < access_order.get_length(); ++i)
    {
        if (access_order.get(i) == key)
        {
            access_order.remove_at(i);
            break;
        }
    }
    access_order.append_element(key);

    if (access_order.get_length() > capacity)
    {
        t_key oldest_key = access_order.get(0);
        table.remove(oldest_key);
        access_order.remove_at(0);
    }
}

template <typename t_key, typename t_value>
void cache<t_key, t_value>::write_to_stream(const t_key &key, const t_value &value)
{
    stream.reset();
    
    while (true)
    {
        try
        {
            stream.read();
        }
        catch (...)
        {
            break;
        }
    }
    
    stream.write(entry<t_key, t_value>(key, value));
}

template <typename t_key, typename t_value>
bool cache<t_key, t_value>::read_from_stream(const t_key &key, t_value &value)
{
    stream.reset();
    
    while (true)
    {
        try
        {
            auto entry_data = stream.read();
            if (entry_data.key == key)
            {
                value = entry_data.value;
                return true;
            }
        }
        catch (...)
        {
            break;
        }
    }
    
    return false;
}