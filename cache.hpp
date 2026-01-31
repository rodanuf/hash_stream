#pragma once

#include "hash_table/hash.hpp"
#include "file_stream/file_stream.hpp"

template <typename t_key, typename t_value>
class cache
{
private:
    hash_table<t_key, t_value> table;
    file_stream<entry<t_key, t_value>> stream;

    array_sequence<t_key> access_order;

    int capacity;
    int hit_count;
    int miss_count;

public:
    cache(int cap, const std::function<int(const t_key&)> &hash_func, const std::string &stream_path);
    ~cache() = default;

    t_value get(const t_key &key);

    void put(const t_key &key, const t_value &value);
    void reset_statistics();

    int get_hit_count() const;
    int get_miss_count() const;
    int get_size() const;

    double get_hit_ratio() const;

private:
    void update_access_order(const t_key &key);
    void write_to_stream(const t_key &key, const t_value &value);

    bool read_from_stream(const t_key &key, t_value &value);

};

#include "cache.tpp"