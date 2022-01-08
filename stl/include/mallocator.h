#ifndef MALLOCATOR_H_
#define MALLOCATOR_H_

#include"mconstruct.h"
#include"mutil.h"

//分配器设计
//allocator   内存 分配 释放 构造 析构
namespace mystl
{

template<class T>
class allocator
{
public:
    typedef T  value_type;
    typedef T* pointer;
    typedef const T* cosnt_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

public:
    //分配
    static T* allocate();
    static T* allocate(size_type n);
    //释放
    static void deallocate(T* ptr);
    static void deallocate(T* ptr,size_type n);
    //构造
    static void construct(T* ptr);    
    static void construct(T* ptr, const T& value);
    static void construct(T* ptr, T&& value);

    template <class... Args>
    static void construct(T* ptr, Args&& ...args);
    //析构
    static void destroy(T* ptr);
    static void destroy(T* first, T* last);
};

//分配
template<class T>
T* allocator<T>::allocate()
{
    return static_cast<T*>(::operator new(sizeof(T)));
}

template<class T>
T* allocator<T>::allocate(size_type n)
{
    if(n==0) return nullptr;
    return static_cast<T*>(::operator new(sizeof(T)*n));
}

//释放
template<class T>
void allocator<T>::deallocate(T* ptr)
{
    if(ptr==nullptr) return;
    ::operator delete(ptr);
}

template<class T>
void allocator<T>::deallocate(T* ptr,size_type n)
{
    if(ptr==nullptr) return;
    ::operator delete(ptr);
}

//构造
template<class T>
void allocator<T>::construct(T* ptr)
{
    mystl::construct(ptr);
}

template<class T>
void allocator<T>::construct(T* ptr, const T& value)
{
    mystl::construct(ptr,value);
}

template<class T>
void allocator<T>::construct(T* ptr,T&& value)
{
    mystl::construct(ptr,mystl::move(value));
}

template<class T>
template<class... Args>
void allocator<T>::construct(T* ptr,Args&& ...args)
{
    mystl::construct(ptr,mystl::forward<Args>(args)...);
}

//上面两个构造函数使用move forward的区别
//std::move用在需要只右值引用的地方；而std::forward用在一个需要统一引用（universal references）的地方（指的是既可以左也可以右的引用）

//析构
template<class T>
void allocator<T>::destroy(T *ptr)
{
    mystl::destory(ptr);
}

template<class T>
void allocator<T>::destroy(T *first,T* last)
{
    mystl::destory(first,last);
}

}
#endif