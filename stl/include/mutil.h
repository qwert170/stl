#ifndef MUTIL_H_
#define MUTIL_H_

#include<cstddef>
#include "mtype_traits.h"
//设计了move forward swap pair
namespace mystl
{
//std::remove_reference<_Tp>用于去除引用部分，只获取 类型部分

//forward 保持其值属性 
//转发左值 t& && -> t&
template<class T>
constexpr T&& forward(typename std::remove_reference<T>::type& t) noexcept
{ return static_cast<T&&>(t); }

//右值转换 t&& -> t&&
template<class T>
constexpr T&& forward(typename std::remove_reference<T>::type&& t) noexcept
{
    static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
    return static_cast<T&&>(t); 
}




//move 转换为右值引用
template<typename T>
typename std::remove_reference<T>::type && move(T&& t) noexcept//使用typename的原因：每当类型名称依赖于模板参数时，都需要 typename 关键字
{
    return static_cast<typename std::remove_reference<T>::type &&>(t);
}



//swap
template<class Tp>
void swap(Tp& lhs,Tp& rhs)
{
    auto tmp(mystl::move(lhs));
    lhs=mystl::move(rhs);
    rhs=mystl::move(tmp);
}

template<class ForwardIter1,class ForwardIter2>
void swap_range(ForwardIter1 first1,ForwardIter1 last1,ForwardIter2 first2)
{
    for(;first1!=last1;++first1,(void)++first2) //使用void是一种标准方法，    ,  通过强制使用内置运算符,来防止用户通过重载其迭代器类型来破坏您的代码。
    {
        mystl::swap(*first1,*first2);
    }
    return first2;
}

template<class Tp,size_t N>
void swap(Tp(&a)[N],Tp(&b)[N])
{
    mystl::swap_range(a,a+N,b);
}



//pair
template<class Ty1,class Ty2>
struct pair
{
    typedef Ty1 first_type;
    typedef Ty2 second_type;

    first_type first;
    second_type second;

    template<class Other1=Ty1,class Other2=Ty2,typename =typename std::enable_if<
    std::is_default_constructible<Other1>::value &&
    std::is_default_constructible<Other2>::value,void>::type>
    constexpr pair():first(),second(){}

    //is_copy_constructible是可复制构造类型  is_convertible检查是否可以将任何数据类型A隐式转换为任何数据类型B
    template<class U1=Ty1,class U2=Ty2,typename std::enable_if<
    std::is_copy_constructible<U1>::value && 
    std::is_copy_constructible<U2>::value && 
    std::is_convertible<const U1&,Ty1>::value && 
    std::is_convertible<const U2&,Ty2>::value,int>::type=0>
    constexpr pair(const Ty1& a,const Ty2& b):first(a),second(b){}

    template<class U1=Ty1,class U2=Ty2,typename std::enable_if<
    std::is_copy_constructible<U1>::value &&
    std::is_copy_constructible<U2>::value &&
    (!std::is_convertible<const U1&,Ty1>::value || !std::is_convertible<const U2&,Ty2>::value),int>::type=0>
    explicit constexpr pair(const Ty1& a,const Ty2& b):first(a),second(b){}

    pair(const pair& rhs)=default;
    pair(pair&& rhs)=default;

    //是否是带有参数集的可构造类型
    template<class Other1,class Other2,typename std::enable_if<
    std::is_constructible<Ty1,Other1>::value &&
    std::is_constructible<Ty2,Other2>::value &&
    std::is_convertible<Other1&&,Ty1>::value&&
    std::is_convertible<Other2&&,Ty2>::value,int>::type=0>
    constexpr pair(Other1&& a,Other2&& b):first(mystl::forward<Other1>(a)),second(mystl::forward<Other2>(b)){}

    template <class Other1, class Other2,typename std::enable_if<
    std::is_constructible<Ty1, Other1>::value &&
    std::is_constructible<Ty2, Other2>::value &&
    (!std::is_convertible<Other1, Ty1>::value || !std::is_convertible<Other2, Ty2>::value), int>::type = 0>
    explicit constexpr pair(Other1&& a, Other2&& b):first(mystl::forward<Other1>(a)),second(mystl::forward<Other2>(b)){}

    //另外一个迭代器
    template<class Other1,class Other2,typename std::enable_if<
    std::is_constructible<Ty1,const Other1&>::value&&
    std::is_constructible<Ty2,const Other2&>::value &&
    std::is_convertible<const Other1&,Ty1>::value &&
    std::is_convertible<const Other2&,Ty2>::value,int>::type=0>
    constexpr pair(const pair<Other1,Other2>& other)
    :first(other.first),second(other.second){}

    template<class Other1,class Other2,typename std::enable_if<
    std::is_constructible<Ty1,const Other1&>::value&&
    std::is_constructible<Ty2,const Other2&>::value &&
    (!std::is_convertible<const Other1&,Ty1>::value ||!std::is_convertible<const Other2&,Ty2>::value),int>::type=0>
    explicit constexpr pair(const pair<Other1,Other2>& other)
    :first(other.first),second(other.second){}

    template<class Other1,class Other2,typename std::enable_if<
    std::is_constructible<Ty1,Other1>::value &&
    std::is_constructible<Ty2,Other2>::value &&
    std::is_convertible<Other1,Ty1>::value&&
    std::is_convertible<Other2,Ty2>::value,int>::type=0>
    constexpr pair(pair<Other1,Other2>&& other):first(mystl::forward<Other1>(other.first)),second(mystl::forward<Other2>(other.second)){}

    template <class Other1, class Other2,typename std::enable_if<
    std::is_constructible<Ty1, Other1>::value &&
    std::is_constructible<Ty2, Other2>::value &&
    (!std::is_convertible<Other1, Ty1>::value || !std::is_convertible<Other2, Ty2>::value), int>::type = 0>
    explicit constexpr pair(pair<Other1,Other2>&& other):first(mystl::forward<Other1>(other.first)),second(mystl::forward<Other2>(other.second)){}

    pair& operator=(const pair& rhs)
    {
        if(this!=&rhs)
        {
            first=rhs.first;
            second=rhs.second;
        }
        return *this;
    }

    pair& operator=(pair&& rhs)
    {
        if(this!=&rhs)
        {
            first=mystl::move(rhs.first);
            second=mystl::move(rhs.second);
        }
        return *this;
    }

    template<class Other1,class Other2>
    pair& operator=(pair<Other1,Other2>&& other)
    {
        first=mystl::forward<Other1>(other.first);
        second=mystl::forward<Other2>(other.second);
        return *this;
    }

    ~pair()=default;

    void swap(pair& other)
    {
        if(this!=&other)
        {
            mystl::swap(first,other.first);
            mystl::swap(second,other.second);
        }
    }
};

template<class Ty1,class Ty2>
bool operator==(const pair<Ty1,Ty2>& lhs,const pair<Ty1,Ty2>& rhs)
{
    return lhs.first==rhs.first&& lhs.second==rhs.second;
}

template<class Ty1,class Ty2>
bool operator<(const pair<Ty1,Ty2>& lhs,const pair<Ty1,Ty2>& rhs)
{
    return lhs.first<rhs.first || (lhs.first==rhs.first&&lhs.second<rhs.second);
}

template<class Ty1,class Ty2>
bool operator!=(const pair<Ty1,Ty2>& lhs,const pair<Ty1,Ty2>& rhs)
{
    return !(lhs==rhs);
}

template<class Ty1,class Ty2>
bool operator>(const pair<Ty1,Ty2>& lhs,const pair<Ty1,Ty2>& rhs)
{
    return rhs<lhs;
}

template<class Ty1,class Ty2>
bool operator<=(const pair<Ty1,Ty2>& lhs,const pair<Ty1,Ty2>& rhs)
{
    return !(rhs<lhs);
}

template<class Ty1,class Ty2>
bool operator>=(const pair<Ty1,Ty2>& lhs,const pair<Ty1,Ty2>& rhs)
{
    return !(lhs<rhs);
}

template<class Ty1,class Ty2>
void swap(pair<Ty1,Ty2>& lhs,pair<Ty1,Ty2>& rhs)
{
    lhs.swap(rhs);
}

template<class Ty1,class Ty2>
pair<Ty1,Ty2>make_pair(Ty1&& first,Ty2&& second)
{
    return pair<Ty1,Ty2>(mystl::forward<Ty1>(first),mystl::forward<Ty2>(second));
}

}

#endif