#include "thread.h"
#include "log.h"
#include "util.h"

namespace sylar
{
static thread_local Thread* t_thread = nullptr;
static thread_local std::string t_thread_name = "UNKONW";
static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");


Semaphore::Semaphore(uint32_t count){
#ifdef __APPLE__
    dispatch_semaphore_create(count);
#else
    sem_init(&s->sem, 0, value);
    if(sem_init(&m_semaphore, 0, count)){
        throw std::logic_error("sem_init error");
    }
#endif
}

Semaphore::~Semaphore(){
#ifdef __APPLE__
    dispatch_release(m_semaphore);
#else
    sem_destroy(&m_semaphore);
#endif
}

void Semaphore::wait(){
#ifdef __APPLE__
    //std::cout << "ccccccc" << std::endl;
    dispatch_semaphore_wait(m_semaphore, DISPATCH_TIME_FOREVER);
    //std::cout << "bbbbbbbbb" << std::endl;
#else
    while (true){
        if(!sem_wait(&m_semaphore)){
            throw std::logic_error("sem_wait error");
        }
    }
#endif
//std::cout << "dddddddddd" << std::endl;
}

void Semaphore::notify(){
#ifdef __APPLE__
    dispatch_semaphore_signal(m_semaphore);
#else
    if(sem_post(&m_semaphore)){
        throw std::logic_error("sem_post error");
    }
#endif

}

Thread* Thread::GetThis(){
    return t_thread;
}

const std::string& Thread::GetName() {
    return t_thread_name;
}

void Thread::SetName(const std::string& name) {
    if(name.empty()) {
        return;
    }
    if(t_thread) {
        t_thread->m_name = name;
    }
    t_thread_name = name;
}


Thread::Thread(std::function<void()> cb, const std::string& name)
    :m_cb(cb)
    ,m_name(name) {
    if(name.empty()){
        m_name = "UNKNOW";
    }

#ifdef __APPLE__
    int rt = pthread_create(&m_thread, nullptr, &Thread::run, NULL);
#else
    int rt = pthread_create(&m_thread, nullptr, &Thread::run, this);
#endif

    if(rt){
        SYLAR_LOG_ERROR(g_logger) << "pthread_create thread fail, rt = " << rt 
             << " name= " << name;
        throw std::logic_error("pthread_create error");
    }
    // std::cout << "aaaaaaaa" << std::endl;
    m_semaphore.wait();
}


Thread::~Thread(){
    if(m_thread){
        pthread_detach(m_thread);
    }
}

void* Thread::run(void* arg){
    Thread* thread = (Thread*)arg;
    t_thread = thread;
    t_thread_name = thread->m_name;
    thread->m_id = sylar::GetThreadId();
#ifdef __linux__
    pthread_setname_np(pthread_self(), thread->m_name.substr(0, 15).c_str());   
#elif __APPLE__
    pthread_setname_np(thread->m_name.substr(0, 15).c_str());
#endif   
    std::function<void()> cb;
    cb.swap(thread->m_cb);

    thread->m_semaphore.notify();

    cb();
    return 0;
}

void Thread::join(){
    if(m_thread){
        int rt = pthread_join(m_thread, nullptr);
        if (rt) {
            SYLAR_LOG_ERROR(g_logger) << "pthread_join thread fail, rt = " << rt;
            throw std::logic_error("pthread_join error");
        }
        m_thread = 0;
    }
}


} // namespace sylar
