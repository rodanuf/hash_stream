#include "cache.hpp"
#include "hash_table/hash.hpp"
#include "file_stream/file_stream.hpp"
#include "benchmark_utils.hpp"
#include <chrono>
#include <fstream>

struct benchmark_result
{
    int cache_size;
    int requests;
    double duration_ms;
    int hits;
    int misses;
    double hit_ratio;
};

benchmark_result run_cache_benchmark(
    int cache_size,
    const array_sequence<int> &workload,
    const std::string &db_file = "cache_db.bin")
{
    auto hash_fn = [](int k)
    { return k % 1000; };

    cache<int, int> my_cache(cache_size, hash_fn, db_file);
    my_cache.reset_statistics();

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < workload.get_length(); i++)
    {
        try
        {
            my_cache.get(workload.get(i));
        }
        catch (...)
        {
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    return {
        cache_size,
        static_cast<int>(workload.get_length()),
        duration.count() / 1000.0, // мс
        my_cache.get_hit_count(),
        my_cache.get_miss_count(),
        my_cache.get_hit_ratio()};
}

void benchmark_scenario()
{
    generate_database<int, int>("cache_db.bin", 10000);

    auto workload = generate_workload(1000, 80);

    array_sequence<int> cache_sizes = {5, 10, 20, 50, 100, 200, 500};
    array_sequence<benchmark_result> results;

    for (int i = 0; i < cache_sizes.get_length(); i++)
    {
        results.append_element(run_cache_benchmark(cache_sizes.get(i), workload));
    }

    std::ofstream csv("cache_benchmark.csv");
    csv << "cache_size,requests,duration_ms,hits,misses,hit_ratio\n";
    for (int i = 0; i < results.get_length(); i++)
    {
        const auto &r = results.get(i);
        csv << r.cache_size << ","
            << r.requests << ","
            << r.duration_ms << ","
            << r.hits << ","
            << r.misses << ","
            << r.hit_ratio << "\n";
    }
}