#include <gtest/gtest.h>
#include "hash_table/hash_table_iterator.hpp"
#include "hash_table/hash.hpp"

TEST(hashtable, basic_functionality)
{
    auto simple_int_hash = [](const int &key)
    {
        return key % 100;
    };

    hash_table<int, int> hash(simple_int_hash, 8);

    hash.add(4, 4);

    auto iterator = hash.get_keys_iterator();

    EXPECT_EQ(iterator->get_current(), 4);
    EXPECT_FALSE(iterator->has_next());
}