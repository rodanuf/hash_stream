#pragma once

#include "../i_iterator.hpp"
#include "../pointers\uniq_ptr.hpp"
#include "entry.hpp"
#include "../lab3_2ndsem/headers/array_sequence.hpp"
#include "../lab3_2ndsem/headers/list_sequence.hpp"

template <typename t_key, typename t_value>
class hash_table_iterator : public i_iterator<t_key>
{
private:
    const array_sequence<list_sequence<entry<t_key, t_value>>> *buckets;
    int current_bucket;
    int inner_index;

public:
    explicit hash_table_iterator(const array_sequence<list_sequence<entry<t_key, t_value>>> &buckets_ref);

    bool has_next() const override;
    bool next() override;
    bool try_get_current(t_key &element) override;

    t_key get_current() const override;

private:
    bool find_next_non_empty();
};

#include "hash_table_iterator.tpp"