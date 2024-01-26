#include <iostream>
#include "gtest/gtest.h"
#include "asio.hpp"

int main(int argc, char **argv)
{
    std::cout << "Hello" << std::endl;

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

    return 0;
}