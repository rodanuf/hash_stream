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

TEST(hashtable, iterator_multiple_keys_same_bucket)
{
    auto simple_int_hash = [](const int &key)
    {
        return key % 100;
    };

    hash_table<int, int> hash(simple_int_hash, 8);

    hash.add(4, 40);
    hash.add(12, 120);
    hash.add(20, 200); 

    auto iterator = hash.get_keys_iterator();

   array_sequence<int> keys;
    do
    {
        keys.append_element(iterator->get_current());
    } while (iterator->next());

    EXPECT_EQ(keys.get_length(), 3);

}

TEST(hashtable, iterator_multiple_keys_different_buckets)
{
    auto simple_int_hash = [](const int &key)
    {
        return key % 100;
    };

    hash_table<int, int> hash(simple_int_hash, 8);

    hash.add(1, 10);  
    hash.add(9, 90);  
    hash.add(2, 20);  
    hash.add(10, 100); 
    hash.add(3, 30);  

    auto iterator = hash.get_keys_iterator();

    EXPECT_TRUE(iterator->has_next());

   array_sequence<int> keys;
    do
    {
        int key;
        EXPECT_TRUE(iterator->try_get_current(key));
        keys.append_element(key);
    } while (iterator->next());

    EXPECT_EQ(keys.get_length(), 5);

}

TEST(hashtable, iterator_empty_table)
{
    auto simple_int_hash = [](const int &key)
    {
        return key % 100;
    };

    hash_table<int, int> hash(simple_int_hash, 8);

    auto iterator = hash.get_keys_iterator();

    EXPECT_FALSE(iterator->has_next());

    EXPECT_FALSE(iterator->next());
}

TEST(hashtable, iterator_single_element)
{
    auto simple_int_hash = [](const int &key)
    {
        return key % 100;
    };

    hash_table<int, int> hash(simple_int_hash, 8);

    hash.add(5, 50);

    auto iterator = hash.get_keys_iterator();

    EXPECT_EQ(iterator->get_current(), 5);
    EXPECT_FALSE(iterator->has_next());

    EXPECT_FALSE(iterator->next());

    EXPECT_EQ(iterator->get_current(), 5);
}

TEST(hashtable, iterator_sequential_access)
{
    auto simple_int_hash = [](const int &key)
    {
        return key % 100;
    };

    hash_table<int, int> hash(simple_int_hash, 8);

    for (int i = 0; i < 10; ++i)
    {
        hash.add(i, i * 10);
    }

    auto iterator = hash.get_keys_iterator();

   array_sequence<int> keys;
    for (int i = 0; i < 10; ++i)
    {
        keys.append_element(iterator->get_current());
        if (i < 9)
        {
            EXPECT_TRUE(iterator->has_next());
            EXPECT_TRUE(iterator->next());
        }
    }

    EXPECT_EQ(keys.get_length(), 10);

}
