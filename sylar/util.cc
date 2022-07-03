#include <execinfo.h>
#include <sys/time.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include "log.h"
#include "fiber.h"
#include <sys/syscall.h>

namespace sylar
{

sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");

pid_t GetThreadId(){
    // return syscall(SYS_gettid);
    return 1;
}

uint32_t GetFiberId(){

    return sylar::Fiber::GetFiberId();
}

void Backtrace(std::vector<std::string>& bt, int size, int skip){
    void** array = (void**)malloc((sizeof(void*) * size));
    size_t s = ::backtrace(array, size);

    char** strings = backtrace_symbols(array, s);
    if(strings == NULL){
        SYLAR_LOG_INFO(g_logger) << "backtrace_synobls error";
    }

    for (size_t i = skip; i < s; i++) {
        bt.push_back(strings[i]);
    }
    free(strings);
    free(array);
}

std::string BacktraceToString(int size, int skip, const std::string& prefix){
    std::vector<std::string> bt;
    Backtrace(bt, size, skip);
    std::stringstream ss;
    for (size_t i = 0; i < bt.size(); i++) {
        ss << prefix << bt[i] << std::endl;
    }
    return ss.str();
}

/**
 * @brief 获取当前时间的毫秒
 */
uint64_t GetCurrentMS(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000ul  + tv.tv_usec / 1000;
}

/**
 * @brief 获取当前时间的微秒
 */
uint64_t GetCurrentUS(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 * 1000ul + tv.tv_usec / 1000;
}

} // namespace sylar
