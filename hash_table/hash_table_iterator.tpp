#include "hash_table_iterator.hpp"
#include <stdexcept>

template <typename t_key, typename t_value>
hash_table_iterator<t_key, t_value>::hash_table_iterator(const array_sequence<list_sequence<entry<t_key, t_value>>> &buckets_ref)
    : buckets(&buckets_ref), current_bucket(0), inner_index(0)
{
    if (!find_next_non_empty())
    {
        current_bucket = buckets->get_length();
    }
}

template <typename t_key, typename t_value>
bool hash_table_iterator<t_key, t_value>::has_next() const
{
    if (current_bucket >= buckets->get_length())
        return false;

    if (inner_index + 1 < (*buckets)[current_bucket].get_length())
        return true;

    for (int i = current_bucket + 1; i < buckets->get_length(); ++i)
    {
        if ((*buckets)[i].get_length() > 0)
            return true;
    }

    return false;
}

template <typename t_key, typename t_value>
bool hash_table_iterator<t_key, t_value>::next() 
{

    ++inner_index;
    return find_next_non_empty();
}

template <typename t_key, typename t_value>
bool hash_table_iterator<t_key, t_value>::try_get_current(t_key &element)
{
    element = buckets->get(current_bucket).get(inner_index).key;
    return true;
}

template <typename t_key, typename t_value>
t_key hash_table_iterator<t_key, t_value>::get_current() const
{
    return buckets->get(current_bucket).get(inner_index).key;
}

template <typename t_key, typename t_value>
bool hash_table_iterator<t_key, t_value>::find_next_non_empty()
{
    while (current_bucket < buckets->get_length())
    {
        const auto &bucket = buckets->get(current_bucket);
        if (inner_index < bucket.get_length())
        {
            return true;
        }
        ++current_bucket;
        inner_index = 0;
    }

    return false;
}