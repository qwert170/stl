#ifndef MCONSTRUCT_H_
#define MCONSTRUCT_H_

//construct 构造函数  destory析构函数
#include<new>

#include "mtype_traits.h"
#include "mallocator.h"

#ifdef _MSC_VER//Microsoft 的 C 编译器的版本
#pragma warning(push)
#pragma warning(disable : 4100) //4100未引用错误
#endif

namespace mystl
{
//构造函数
template<class Ty>
void construct(Ty *ptr)
{
    ::new ((void *)ptr) Ty();
}

template<class Ty1,class Ty2>
void construct(Ty1 *ptr,const Ty2& value)
{
    ::new ((void *)ptr) Ty1(value);
}

template<class Ty,class... Args>
void construct(Ty *ptr,Args&&... args)
{
    ::new ((void *)ptr) Ty(mystl::forward<Args>(args)...);
}

//析构函数

template<class Ty>
void destory_one(Ty*,std::true_type){}

template<class Ty>
void destory_one(Ty *pointer,std::false_type)
{
    if(pointer!=nullptr)
    {
        pointer->~Ty();//
    }
}

template<class ForwardIter>
void destory_cat(ForwardIter ,ForwardIter,std::true_type){}

template<class ForwardIter>
void destory_cat(ForwardIter first,ForwardIter last,std::false_type)
{
    for(;first!=last;++first)
    {
        destory(&*first);
    }
}

template<class Ty>
void destory(Ty *pointer)
{
    destory_one(pointer,std::is_trivially_destructible<Ty>{});//测试类型是否为普通易损坏类型,可析构类型
}

template<class ForwardIter>
void destory(ForwardIter first,ForwardIter last)
{
    destory_cat(first,last, std::is_trivially_destructible<typename iterator_traits<ForwardIter>::value_type>{});
}

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif