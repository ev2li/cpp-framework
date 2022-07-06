#ifndef __SYLAR_ENDIAN_H__
#define __SYLAR_ENDIAN_H__

#define SYLAR_LITTLE_ENDIAN 1
#define SYLAR_BIG_ENDIAN 2

#include <byteswap.h>
#include <stdint.h>

namespace sylar {

template<class T>
typename std::enable_if<sizeof(T) == sizeof(uint64_t), T>::type
btyeswap(T value){
    return (T)bswap_64((uint64_t));
}


template<class T>
typename std::enable_if<sizeof(T) == sizeof(u_int32_t), T>::type
btyeswap(T value){
    return (T)bswap_32((uint32_t));
}

template<class T>
typename std::enable_if<sizeof(T) == sizeof(u_int16_t), T>::type
btyeswap(T value){
    return (T)bswap_16((uint16_t));
}

#if BYTE_ORDER == BIG_ENDIAN
#define SYLAR_BYTE_ORDER SYLAR_BIG_ENDIAN
#else
#define SYLAR_BYTE_ORDER SYLAR_LITTLE_ENDIAN
#endif

#if SYLAR_BYTE_ORDER == SYLAR_BIG_ENDIAN
template<class T>
T byteswapOnLittleEndian(T t){
    return t;
}

template<class T>
T byteswapOnBigEndian(T t){
    return byteswap(t);
}
#else
template<class T>
T byteswapOnLittleEndian(T t){
    return byteswap(t);
}

template<class T>
T byteswapOnBigEndian(T t){
    return t;
}
#endif

} /* end ns sylar */

#endif /* __SYLAR_ENDIAN_H__ */
