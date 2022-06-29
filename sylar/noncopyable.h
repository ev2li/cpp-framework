#ifndef __SYLAR_NONCOPYABLE__H__
#define __SYLAR_NONCOPYABLE__H__

namespace sylar{

class Noncopyable{
public:
    Noncopyable() = default;
    ~Noncopyable() = default;

    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
};
    
} // namespace sylar


#endif /* __SYLAR_NONCOPYABLE__H__ */
