#include <stdio.h>
#include "gtest/gtest.h"


#include <Alime/base/json/aJson.h>

int main(int argc, char** argv)
{
    {
        AlimeJson aj;
        std::string str = R"("hello": "world")";
        aj = aj.Parse(str.c_str());
        auto ty=aj.GetType();
        
    }
    printf("Running main() from gtest_main.cc\n");
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
