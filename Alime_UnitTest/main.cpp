#define _CRTDBG_MAP_ALLOC 
#include <stdio.h>
#define new  new(_CLIENT_BLOCK, __FILE__, __LINE__)  
#include "gtest/gtest.h"


#include "windows.h"


int main(int argc, char** argv)
{
    int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

    tmpFlag |= _CRTDBG_LEAK_CHECK_DF;

    _CrtSetDbgFlag(tmpFlag);
    _CrtSetBreakAlloc(4625);

    printf("Running main() from gtest_main.cc\n");
    testing::InitGoogleTest(&argc, argv);
    
   
    RUN_ALL_TESTS();
}
