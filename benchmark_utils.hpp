#pragma once

#include "file_stream/file_stream.hpp"
#include "hash_table/entry.hpp"
#include <random>
#include <vector>

template <typename t_key, typename t_value>
void generate_database(const std::string &file_path, int size)
{
    file_stream<entry<t_key, t_value>> stream(file_path);
    stream.move_position(0);

    const int HOT_KEYS = 50;
    const int COLD_KEYS = 100;
    const int COLD_KEYS_START = 1000;

    for (int i = 0; i < HOT_KEYS; ++i)
    {
        stream.write(entry<t_key, t_value>{
            static_cast<t_key>(i),
            static_cast<t_value>(i * 1000 + 42)
        });
    }

    for (int i = 0; i < COLD_KEYS; ++i)
    {
        stream.write(entry<t_key, t_value>{
            static_cast<t_key>(COLD_KEYS_START + i),
            static_cast<t_value>(i * 2000 + 123)});
    }

    std::mt19937 gen(42);
    std::uniform_int_distribution<> cold_key_dist(2000, 20000);
    std::uniform_int_distribution<> val_dist(0, 1000000);

    for (int i = HOT_KEYS + COLD_KEYS; i < size; ++i)
    {
        t_key key = static_cast<t_key>(cold_key_dist(gen));
        t_value value = static_cast<t_value>(val_dist(gen));
        stream.write(entry<t_key, t_value>{key, value});
    }
    stream.reset();
}

array_sequence<int> generate_workload(int total_requests)
{
    array_sequence<int> workload;

    std::mt19937 gen(123);
    std::uniform_real_distribution<> prob_dist(0.0, 1.0);

    const int HOT_KEYS = 50;
    const int COLD_KEYS = 100;
    const int COLD_KEYS_START = 1000;

    for (int i = 0; i < total_requests; ++i)
    {
        double p = prob_dist(gen);
        if (p < 0.85)
        {
            int hot_key = gen() % HOT_KEYS;
            workload.append_element(hot_key);
        }
        else
        {
            int cold_key = COLD_KEYS_START + (gen() % COLD_KEYS);
            workload.append_element(cold_key);
        }
    }

    return workload;
}