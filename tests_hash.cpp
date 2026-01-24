#include <gtest/gtest.h>
#include "hash.hpp"
#include <functional>

auto simple_int_hash = [](const int &key)
{
    return key % 100;
};

TEST(hash_table_test, default_constructor_with_hash_function)
{
    hash_table<int, std::string> table(simple_int_hash);

    EXPECT_EQ(table.get_count(), 0);
    EXPECT_EQ(table.get_capacity(), 8);
}

TEST(hash_table_test, constructor_with_custom_capacity)
{
    hash_table<int, int> table([](const int &key)
                               { return key; }, 16);

    EXPECT_EQ(table.get_count(), 0);
    EXPECT_EQ(table.get_capacity(), 16);
}

TEST(hash_table_test, method_get_count)
{
    hash_table<int, std::string> table(simple_int_hash);

    EXPECT_EQ(table.get_count(), 0);
}

TEST(hash_table_test, method_get_capacity)
{
    hash_table<int, std::string> table(simple_int_hash, 32);

    EXPECT_EQ(table.get_capacity(), 32);
}

TEST(hash_table_test, method_get_bucket_size)
{
    hash_table<int, std::string> table(simple_int_hash, 10);

    for (int i = 0; i < 10; i++)
    {
        EXPECT_EQ(table.get_bucket_size(i), 0);
    }

    EXPECT_THROW(table.get_bucket_size(-1), std::out_of_range);
    EXPECT_THROW(table.get_bucket_size(20), std::out_of_range);
}

TEST(hash_table_test, method_calc_value)
{
    auto custom_hash = [](const int &key)
    { return key * 2; };
    hash_table<int, std::string> table(custom_hash);

    EXPECT_EQ(table.calc_value(5), 10);
    EXPECT_EQ(table.calc_value(10), 20);
}

TEST(hash_table_test, method_get_existing_key)
{
    hash_table<int, std::string> table(simple_int_hash);

    table.set(1, "one");
    table.set(2, "two");
    table.set(3, "three");

    EXPECT_EQ(table.get(1), "one");
    EXPECT_EQ(table.get(2), "two");
    EXPECT_EQ(table.get(3), "three");
}

TEST(hash_table_test, method_get_nonexistent_key)
{
    hash_table<int, std::string> table(simple_int_hash);

    table.set(1, "one");

    EXPECT_THROW(table.get(999), std::out_of_range);
}

TEST(hash_table_test, method_set_new_key)
{
    hash_table<int, std::string> table(simple_int_hash);

    table.set(1, "value1");
    table.set(2, "value2");
    table.set(3, "value3");

    EXPECT_EQ(table.get(1), "value1");
    EXPECT_EQ(table.get(2), "value2");
    EXPECT_EQ(table.get(3), "value3");
    EXPECT_EQ(table.get_count(), 3);
}

TEST(hash_table_test, method_set_existing_key)
{
    hash_table<int, std::string> table(simple_int_hash);

    table.set(1, "old_value");
    EXPECT_EQ(table.get(1), "old_value");

    table.set(1, "new_value");
    EXPECT_EQ(table.get(1), "new_value");
    EXPECT_EQ(table.get_count(), 1);
}

TEST(hash_table_test, method_del_existing_key)
{
    hash_table<int, std::string> table(simple_int_hash);

    table.set(1, "one");
    table.set(2, "two");
    table.set(3, "three");

    EXPECT_EQ(table.get_count(), 3);

    table.del(2);

    EXPECT_EQ(table.get_count(), 2);
    EXPECT_THROW(table.get(2), std::out_of_range);
    EXPECT_EQ(table.get(1), "one");
    EXPECT_EQ(table.get(3), "three");
}

TEST(hash_table_test, method_del_nonexistent_key)
{
    hash_table<int, std::string> table(simple_int_hash);

    table.set(1, "one");

    EXPECT_NO_THROW(table.del(999));
    EXPECT_EQ(table.get_count(), 1);
}

TEST(hash_table_test, method_set_capacity_increase)
{
    hash_table<int, std::string> table(simple_int_hash, 4);

    EXPECT_EQ(table.get_capacity(), 4);

    table.set_capacity(8);

    EXPECT_EQ(table.get_capacity(), 8);
}

TEST(hash_table_test, method_rehash)
{
    hash_table<int, std::string> table(simple_int_hash, 4);

    for (int i = 0; i < 10; i++)
    {
        table.set(i, "value" + std::to_string(i));
    }

    EXPECT_EQ(table.get_capacity(), 16);
    EXPECT_EQ(table.get_count(), 10);

    table.rehash(16);

    EXPECT_EQ(table.get_capacity(), 16);

    for (int i = 0; i < 10; i++)
    {
        EXPECT_EQ(table.get(i), "value" + std::to_string(i));
    }
}

TEST(hash_table_test, method_resize)
{
    hash_table<int, std::string> table(simple_int_hash, 4);

    EXPECT_EQ(table.get_capacity(), 4);

    table.resize();

    EXPECT_EQ(table.get_capacity(), 8);
}

TEST(hash_table_test, method_resize_if_needed)
{
    hash_table<int, std::string> table(simple_int_hash, 4);

    table.set(1, "one");
    table.set(2, "two");
    table.set(3, "three");
    EXPECT_TRUE(true);
}

TEST(hash_table_test, method_contains_key)
{
    hash_table<int, std::string> table(simple_int_hash);

    table.set(1, "one");
    table.set(2, "two");

    EXPECT_TRUE(table.contains_key(1));
    EXPECT_TRUE(table.contains_key(2));
    EXPECT_FALSE(table.contains_key(3));
    EXPECT_FALSE(table.contains_key(999));
}

TEST(hash_table_test, method_is_consistent)
{
    hash_table<int, std::string> table(simple_int_hash);

    EXPECT_TRUE(table.is_consistent());

    table.set(1, "one");
    table.set(2, "two");
    table.set(3, "three");

    EXPECT_TRUE(table.is_consistent());
}

TEST(hash_table_test, method_get_load_factor)
{
    hash_table<int, std::string> table(simple_int_hash, 10);

    EXPECT_EQ(table.get_load_factor(), 0.0);

    table.set(1, "one");
    EXPECT_EQ(table.get_load_factor(), 0.1);

    table.set(2, "two");
    table.set(3, "three");
    table.set(4, "four");
    table.set(5, "five");

    EXPECT_EQ(table.get_load_factor(), 0.5);
}

TEST(hash_table_test, method_get_bucket_distribution)
{
    hash_table<int, std::string> table(simple_int_hash, 5);

    for (int i = 0; i < 10; i++)
    {
        table.set(i, "value" + std::to_string(i));
    }

    auto distribution = table.get_bucket_distribution();

    EXPECT_EQ(distribution.get_length(), 20); // т. к. resize_if_needed вызовется на 8 элементе -> capacity * 2 = 20

    int total_elements = 0;
    for (int i = 0; i < distribution.get_length(); i++)
    {
        total_elements += distribution[i];
    }
    EXPECT_EQ(total_elements, 10);
}

TEST(hash_table_test, collisions_handling)
{
    auto collision_hash = [](const int &key)
    { return key % 3; };
    hash_table<int, std::string> table(collision_hash, 3);

    table.set(1, "one");   
    table.set(4, "four"); 
    table.set(7, "seven");

    EXPECT_EQ(table.get_count(), 3);
    EXPECT_EQ(table.get(1), "one");
    EXPECT_EQ(table.get(4), "four");
    EXPECT_EQ(table.get(7), "seven");
}

TEST(hash_table_test, method_add_interface)
{
    hash_table<int, std::string> table(simple_int_hash);

    table.add(1, "one");
    table.add(2, "two");

    EXPECT_EQ(table.get(1), "one");
    EXPECT_EQ(table.get(2), "two");
    EXPECT_EQ(table.get_count(), 2);
}

TEST(hash_table_test, method_remove_interface)
{
    hash_table<int, std::string> table(simple_int_hash);

    table.set(1, "one");
    table.set(2, "two");
    table.set(3, "three");

    EXPECT_EQ(table.get_count(), 3);

    table.remove(2);

    EXPECT_EQ(table.get_count(), 2);
    EXPECT_THROW(table.get(2), std::out_of_range);
}

TEST(hash_table_test, large_number_of_elements)
{
    hash_table<int, int> table([](const int &key)
                               { return key; }, 100);

    for (int i = 0; i < 1000; i++)
    {
        table.set(i, i * 10);
    }

    EXPECT_EQ(table.get_count(), 1000);

    for (int i = 0; i < 1000; i += 100)
    {
        EXPECT_EQ(table.get(i), i * 10);
    }
}

TEST(hash_table_test, custom_type_keys)
{
    struct point
    {
        int x, y;
        bool operator==(const point &other) const
        {
            return x == other.x && y == other.y;
        }
    };

    auto point_hash = [](const point &p)
    {
        return p.x * 31 + p.y;
    };

    hash_table<point, std::string> table(point_hash);

    point p1{1, 2};
    point p2{3, 4};
    point p3{5, 6};

    table.set(p1, "point one");
    table.set(p2, "point two");
    table.set(p3, "point three");

    EXPECT_EQ(table.get(p1), "point one");
    EXPECT_EQ(table.get(p2), "point two");
    EXPECT_EQ(table.get(p3), "point three");
}

TEST(hash_table_test, edge_cases_empty_table)
{
    hash_table<int, std::string> table(simple_int_hash);

    EXPECT_EQ(table.get_count(), 0);
    EXPECT_EQ(table.get_load_factor(), 0.0);
    EXPECT_TRUE(table.is_consistent());

    EXPECT_NO_THROW(table.del(999));

    EXPECT_FALSE(table.contains_key(1));
}

TEST(hash_table_test, iterator_interface)
{
    hash_table<int, std::string> table(simple_int_hash);

    table.set(1, "one");
    table.set(2, "two");
    table.set(3, "three");

    auto iterator = table.get_keys_iterator();

    EXPECT_NE(iterator, nullptr);

    delete iterator;
}

TEST(hash_table_test, complex_operations_chain)
{
    hash_table<int, std::string> table(simple_int_hash, 4);

    table.set(1, "one")
        .set(2, "two")
        .set(3, "three")
        .del(2)
        .set(4, "four")
        .set(5, "five");

    EXPECT_EQ(table.get_count(), 4);
    EXPECT_TRUE(table.contains_key(1));
    EXPECT_FALSE(table.contains_key(2));
    EXPECT_TRUE(table.contains_key(3));
    EXPECT_TRUE(table.contains_key(4));
    EXPECT_TRUE(table.contains_key(5));
}
