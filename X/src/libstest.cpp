#include "gtest/gtest.h"
#include <nlohmann/json.hpp>
#include <iostream>

using namespace std;

using namespace nlohmann;
TEST(json, json)
{
    json j;
    j["id"] = "test";

    string s = j.dump();
    cout << s << endl;
}