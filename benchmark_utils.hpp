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

    std::mt19937 gen(42); 
    std::uniform_int_distribution<> key_dist(0, size - 1);
    std::uniform_int_distribution<> val_dist(0, 1000000);

    for (int i = 0; i < size; ++i)
    {
        t_key key = static_cast<t_key>(key_dist(gen));
        t_value value = static_cast<t_value>(val_dist(gen));
        stream.write(entry<t_key, t_value>{key, value});
    }
    stream.reset();
}

array_sequence<int> generate_workload(int requests, int hot_keys_ratio = 80)
{
    array_sequence<int> workload(requests);

    std::mt19937 gen(123);
    std::uniform_int_distribution<> dist(0, 99); // 0-79 = горячие, 80-99 = холодные

    for (int i = 0; i < requests; ++i)
    {
        int r = dist(gen);
        if (r < hot_keys_ratio)
        {
            // Горячий ключ (повторяется часто)
            workload.append_element(r % 20); 
        }
        else
        {
            // Холодный ключ (редко повторяется)
            workload.append_element(100 + (r % 80)); 
        }
    }
    return workload;
}