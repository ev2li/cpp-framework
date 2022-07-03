#include "hook.h"
#include "fiber.h"
#include "iomanager.h"

#include "hook.h"
#include <dlfcn.h>

#include "config.h"
#include "log.h"
#include "fiber.h"
#include "iomanager.h"
#include "macro.h"

sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");
namespace sylar
{

static thread_local bool t_hook_enable = false;

#define HOOK_FUN(XX) \
    XX(sleep) \
    XX(usleep) \
    XX(nanosleep) \
    XX(socket) \
    XX(connect) \
    XX(accept) \
    XX(read) \
    XX(readv) \
    XX(recv) \
    XX(recvfrom) \
    XX(recvmsg) \
    XX(write) \
    XX(writev) \
    XX(send) \
    XX(sendto) \
    XX(sendmsg) \
    XX(close) \
    XX(fcntl) \
    XX(ioctl) \
    XX(getsockopt) \
    XX(setsockopt)

void hook_init(){
    static bool is_inited = false;
    if(is_inited){
        return;
    }
#define XX(name) name ## _f = (name ## _fun)dlsym(RTLD_NEXT, #name);
    HOOK_FUN(XX);
#undef XX
}

struct _HookIniter
{
    _HookIniter(){
        hook_init();
    }
};

static _HookIniter s_hook_initer;

bool is_hook_enable(){
    return t_hook_enable;
}

void set_hook_enable(bool flag){
    t_hook_enable = flag;
}

} // namespace sylar

extern "C"{

#define XX(name) name ## _fun name ## _f = nullptr;
    HOOK_FUN(XX);
#undef XX
unsigned sleep(unsigned int seconds){
    if(!sylar::t_hook_enable){
        return sleep_f(seconds);
    }

    sylar::Fiber::ptr fiber = sylar::Fiber::GetThis();
    sylar::IOManager* iom = sylar::IOManager::GetThis();
    iom->addTimer(seconds * 1000, [iom, fiber](){
        iom->schedule(fiber);
    });
    sylar::Fiber::YieldToHold();
    return 0;
}

int usleep(useconds_t usec){
    if(!sylar::t_hook_enable){
        return usleep_f(usec);
    }

    sylar::Fiber::ptr fiber = sylar::Fiber::GetThis();
    sylar::IOManager* iom = sylar::IOManager::GetThis();

    iom->addTimer(usec / 1000, [iom, fiber](){
        iom->schedule(fiber);
    });

    sylar::Fiber::YieldToHold();
    return 0;
}

int nanosleep(const struct timespec *req, struct timespec *rem){
    if(!sylar::t_hook_enable){
        return nanosleep_f(req, rem);
    }

    int timeout_ms = req->tv_sec * 1000 + req->tv_nsec / 1000 / 1000;

}

ssize_t read(int fd, void *buf, size_t count){

}


ssize_t readv(int fd, const struct iovec *iov, int iovcnt){

}


ssize_t (*recv_fun)(int sockfd, void *buf, size_t len, int flags);


ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen){

}


ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags){

}


//write
ssize_t write_fun(int fd, const void *buf, size_t count){

}


ssize_t writev(int fd, const struct iovec *iov, int iovcnt){

}


ssize_t send(int s, const void *msg, size_t len, int flags){

}


ssize_t sendto(int s, const void *msg, size_t len, int flags, const struct sockaddr *to, socklen_t tolen){

}


ssize_t sendmsg(int s, const struct msghdr *msg, int flags){

}


int close(int fd){

}


//
int fcntl_fun(int fd, int cmd, ... /* arg */ ){

}

int ioctl_fun(int d, unsigned long int request, ...){

}


int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen){

}


int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen){

}

}
