#include <stdio.h>
#include "gtest/gtest.h"


#include "windows.h"


int main(int argc, char** argv)
{
    int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

    tmpFlag |= _CRTDBG_LEAK_CHECK_DF;

    _CrtSetDbgFlag(tmpFlag);
    int* leak = new int[3];
    leak[0] = 0xFFFFFFFF;
    leak[1] = 0;
    leak[2] = 0xFFFFFFFF;

    printf("Running main() from gtest_main.cc\n");
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
