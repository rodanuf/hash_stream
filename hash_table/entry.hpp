#pragma once

template <typename t_key, typename t_value>
struct entry
{
    t_key key;
    t_value value;

    entry(const t_key &key, const t_value &value) : key(key), value(value) {}
};