#pragma once

#include "lab3_2ndsem/headers/array_sequence.hpp"
#include "lab3_2ndsem/headers/list_sequence.hpp"
#include "pointers/shared_ptr.hpp"
#include <functional>

template <typename t_key, typename t_value>
class hash_table : public i_dictionary<t_key, t_value> 
{
private:
    struct entry 
    {
        t_key key;
        t_value value;

        entry(const t_key &key, const t_value &value);
    };

    array_sequence<list_sequence<entry>> buckets;
    int count;
    int capacity;

    std::function<int (const t_key &)> hash_function;

public:
    hash_table(const std::function<int (const t_key &)> &hash_function, int capacity = 8);
    ~hash_table() = default;

    int get_count() const override;
    int get_capacity() const override;
    int get_bucket_size(int index) const;   

    const t_value &get(const t_key &key) const override;

    bool contains_key(const t_key &key) const override;

    hash_table<t_key, t_value> &set(const t_key &key, const t_value &value);
    hash_table<t_key, t_value> &del(const t_key &key);

    void add(const t_key &key, const t_value &value) override;
    void remove(const t_key &key) override;

    bool try_get(const t_key &key, t_value &value) const;
    bool try_add(const t_key &key, const t_value &value);
    bool try_remove(const t_key &key);
    bool try_update(const t_key &key, const t_value &value);
    bool is_consistent() const;

    double get_load_factor() const;

    hash_table<t_key, t_value> &set_capacity(int new_capacity);
    hash_table<t_key, t_value> &rehash(int new_capacity);

    int hash(const t_key &key) const;   

    array_sequence<int> get_bucket_distribution() const

    template <typename U>
    hash_table<t_key, U> map(std::function<U(const t_value &)> func) const;

    U reduce(U initial_value, std::function<U(U, const t_value &)> func) const;

    hash_table<t_key, t_value> where(std::function<bool(const t_value &)> predicate) const;

    hash_table<t_key, t_value>& filter(std::function<bool(const t_value &)> predicate);
    hash_table<t_key, t_value>& map_mutable(std::function<t_value(const t_value &)> func);

    list_sequence<t_key> select_keys() const;
    list_sequence<t_value> select_values() const;

    i_iterator<t_key> *get_keys_iterator() override;

private:
    shared_ptr<entry> find_in_bucket(int index, const t_key &key);

};

#include "hash.tpp"