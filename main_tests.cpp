#include <gtest/gtest.h>
#include "hash.hpp"

#include "tests_hash.cpp"

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}