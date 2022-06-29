#include "sylar/sylar.h"
#include <assert.h>

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

void test_assert(){
    SYLAR_LOG_INFO(g_logger) << sylar::BacktraceToString(10);
    SYLAR_ASSERT2(0 == 1, "abcdefg xxx");
}

int main(int argc, char const *argv[])
{
    // assert(1);
    // assert(0);
    test_assert();
    return 0;
}
