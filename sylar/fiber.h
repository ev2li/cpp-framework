#ifndef __SYLAR_FIBER_H__
#define __SYLAR_FIBER_H__
#include <ucontext.h>
#include <memory>
#include "thread.h"
#include <functional>
#define _XOPEN_SOURCE 600 

namespace sylar
{
class Scheduler;
class Fiber: public std::enable_shared_from_this<Fiber> {
friend class Scheduler;
public:
    typedef std::shared_ptr<Fiber> ptr;

    enum State{
        INIT,
        HOLD,
        EXEC,
        TERM,
        READY,
        EXCEPT
    };

private:
    Fiber();

public:
    Fiber(std::function<void()>cb, size_t stacksize = 0);
    ~Fiber();

    /*
        @brief 重置协程函数,并重置状态 INIT TERM
    */
    void reset(std::function<void()> cb);
    /*
        @brief 切换到当前协程执行
    */
    void swapIn();
    /*
        @brief 切换到后台执行
    */
    void swapOut();

    uint64_t getId() const { return m_id; }

    /**
     * @brief 返回协程状态
     */
    State getState() const { return m_state;}

public:
    /**
     * @brief 设置当前线程的运行协程
     * @param[in] f 运行协程
     */
    static void SetThis(Fiber* f);

    /**
     * @brief 返回当前所在的协程
     */
    static Fiber::ptr GetThis();

    /**
     * @brief 将当前协程切换到后台,并设置为READY状态
     * @post getState() = READY
     */
    static void YieldToReady();

    /**
     * @brief 将当前协程切换到后台,并设置为HOLD状态
     * @post getState() = HOLD
     */
    static void YieldToHold();

    /**
     * @brief 返回当前协程的总数量
     */
    static uint64_t TotalFibers();

    static void MainFunc();

    static uint64_t GetFiberId();

private:
    uint64_t m_id = 0;
    uint32_t m_stacksize = 0;;
    State m_state = INIT;
    ucontext_t m_ctx;
    void* m_stack = nullptr;

    std::function<void()> m_cb;
};

} // namespace sylar

#endif /* __SYLAR_FIBER_H__ */
