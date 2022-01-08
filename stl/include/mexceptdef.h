#ifndef MEXCEPTDEF_H_
#define MEXCEPTDEF_H_

#include<stdexcept>
#include<cassert>

namespace mystl
{
    #define MYSTL_DEBUG(expr) \
        assert(expr)

    //长度错误。它报告由于试图超出某些对象的实现定义的长度限制而导致的错误
    #define THROW_LENGTH_ERROR_IF(expt,what) \
        if((expr)) throw std::length_error(what)

    //它报告试图访问定义范围之外的元素所导致的错误。
    #define THROW_OUT_OF_RANGE_IF(expr,what) \
        if((expr)) throw std::out_of_range(what)
        
    //于程序范围以外的事件造成的,而且不易预测。
    #define THROW_RUNTIME_ERROR_IF(expr,what) \
        if((expr)) throw std::runtime_error(what)
}

#endif