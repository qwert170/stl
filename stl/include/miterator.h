#ifndef MITERATOR_H_
#define MITERATOR_H_
/*设计迭代器 五种迭代器类型 计算距离 移动迭代器 一些基本操作*/
#include<cstddef>
#include "mtype_traits.h"

namespace mystl
{
//五种迭代器
struct input_iterator_tag{};//输入
struct output_iterator_tag{};//输出
struct forward_iterator_tag: public input_iterator_tag{};//正向 只继承input的
struct bidirectional_iterator_tag:public forward_iterator_tag{};//双向
struct random_access_iterator_tag:public bidirectional_iterator_tag{};//随机访问

//iterator模板
template<class Category,class T,class Distance=ptrdiff_t,class Pointer=T*,class Reference=T&>
struct iterator
{
    typedef Category   iterator_category;//迭代器种类
    typedef T          value_type;//迭代器所指对象的类型
    typedef Pointer    pointer;
    typedef Reference    reference;
    typedef Distance difference_type;
};

//判断是否有迭代器类别
template<class T>
struct has_iterator_cat
{
private:
    struct two{char a;char b;};//这里是2 
    template<class U>
    static two test(...);
    template<class U>
    static char test(typename U::iterator_category*=0);
public:
    static const bool value =sizeof(test<T>(0))==sizeof(char);
};

//impl 有了迭代器种类后，得到所有迭代器信息
template<class Iterator,bool>
struct iterator_traits_impl{};

template<class Iterator>
struct iterator_traits_impl<Iterator,true>
{
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type  value_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
    typedef typename Iterator::difference_type difference_type;
};

//没有迭代器类型
template<class Iterator,bool>
struct iterator_traits_helper{};

//判断迭代器类型是否可隐式转换
template<class Iterator>
struct iterator_traits_helper<Iterator,true>:public iterator_traits_impl<Iterator,
std::is_convertible<typename Iterator::iterator_category,input_iterator_tag>::value||
std::is_convertible<typename Iterator::iterator_category,output_iterator_tag>::value>{};

//萃取迭代器特性
template<class Iterator>
struct iterator_traits:public iterator_traits_helper<Iterator,has_iterator_cat<Iterator>::value>{};

//针对原生指针T*生成的traits偏特化
template<class T>
struct iterator_traits<T*>
{
    typedef random_access_iterator_tag  iterator_category;
    typedef T  value_type;
    typedef T*  pointer;
    typedef T&  reference;
    typedef ptrdiff_t difference_type;
};

//const T*
template<class T>
struct iterator_traits<const T*>
{
    typedef random_access_iterator_tag  iterator_category;
    typedef T  value_type;
    typedef const T*  pointer;
    typedef const T&  reference;
    typedef ptrdiff_t difference_type;
};

//T迭代器能转换成U迭代器
template<class T,class U,bool=has_iterator_cat<iterator_traits<T>>::value>
struct has_iterator_cat_of:
public m_bool_constant<std::is_convertible<typename iterator_traits<T>::iterator_category,U>::value> {};

//T不能转换成U迭代器
template<class T,class U>
struct has_iterator_cat_of<T,U,false>:public m_false_type{};

//判断迭代器具体类型
template<class Iter>
struct is_input_iterator:public has_iterator_cat_of<Iter,input_iterator_tag>{};

template<class Iter>
struct is_output_iterator:public has_iterator_cat_of<Iter,output_iterator_tag>{};

template<class Iter>
struct is_forward_iterator:public has_iterator_cat_of<Iter,forward_iterator_tag> {};

template <class Iter>
struct is_bidirectional_iterator : public has_iterator_cat_of<Iter, bidirectional_iterator_tag> {};

template <class Iter>
struct is_random_access_iterator : public has_iterator_cat_of<Iter, random_access_iterator_tag> {};

//判断是否是迭代器
template<class Iterator>
struct is_iterator:
public m_bool_constant<is_input_iterator<Iterator>::value||is_output_iterator<Iterator>::value>{};

//萃取迭代器类型category
template<class Iterator>
typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&)
{
    typedef typename iterator_traits<Iterator>::iterator_category Category;
    return Category();
}

//萃取迭代器之间距离distance_type
template<class Iterator>
typename iterator_traits<Iterator>::difference_type*
difference_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

//萃取迭代器指向值的类型value_type
template <class Iterator>
typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&)
{
  return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

//计算距离
//按照迭代器种类进行处理
template<class InputIterator>
typename iterator_traits<InputIterator>::difference_type distance_dispatch(InputIterator first,InputIterator last,input_iterator_tag)
{
    typename iterator_traits<InputIterator>::difference_type n=0;
    while(first!=last)
    {
        ++first;
        ++n;
    }
    return n;
}

template<class InputIterator>
typename iterator_traits<InputIterator>::difference_type distance_dispatch(InputIterator first,InputIterator last,random_access_iterator_tag)
{
    return last-first;
}

template<class InputIterator>
typename iterator_traits<InputIterator>::difference_type diatance(InputIterator first,InputIterator last)
{
    return distance_dispatch(first,last,iterator_category(first));
}


//移动迭代器  双向可以后退 随机访问是指针，也可以后退
template<class InputIterator,class Distance>
void advance_dispatch(InputIterator &i,Distance n,input_iterator_tag)
{
    while(n--)
    {
        i++;
    }
}

//random_access_iterator_tag  随机
template<class InputIterator,class Distance>
void advance_dispatch(InputIterator &i,Distance n,random_access_iterator_tag)
{
    i+=n;
}

//bidirectional_iterator_tag  双向
template<class InputIterator,class Distance>
void advance_dispatch(InputIterator &i,Distance n,bidirectional_iterator_tag)
{
    if(n>=0)
    {
        while(n--)
        {
            ++i;
        }
    }else{
        while(++n)
        {
            --i;
        }
    }
}

template<class InputIterator,class Distance>
void advance(InputIterator &i,Distance n)
{
    advance_dispatch(i,n,iterator_category(i));
} 

//反向迭代器
template<class Iterator>
class reverse_iterator
{
private:
    Iterator current;
public:
    typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
    typedef typename iterator_traits<Iterator>::value_type value_type;
    typedef typename iterator_traits<Iterator>::difference_type difference_type;
    typedef typename iterator_traits<Iterator>::pointer pointer;
    typedef typename iterator_traits<Iterator>::reference reference;

    typedef Iterator iterator_type;
    typedef reverse_iterator<Iterator>   self;

public:
    reverse_iterator(){}
    explicit reverse_iterator(iterator_type i):current(i){}
    reverse_iterator(cosnt self& rhs):current(rhs.current){}
public:
    //取出对应的正向迭代器
    iterator_type base() const {return current;}
    
    //正向迭代器的前一个位置
    reference operator*() const 
    {
        auto temp=current;
        return *--tmp;
    }
    pointer operator->() const
    {
        return &(operator*());
    }
    //--
    self& operator--() const//前置
    {
        ++current;
        return *this;
    }
    self operator--(int) const//后置
    {
        self tmp=*this;
        ++current;
        return tmp;
    }

    //++
    self& operator++() const
    {
        --current;
        return *this;
    }
    self operator++(int) const
    {
        self temp=*this;
        --current;
        return temp;
    }

    self& operator+=(difference_type n) const
    {
        current-=n;
        return *this;
    }

    self operator+(difference_type n) const
    {
        return self(current-n);
    }

    self& operator-=(difference_type n) const
    {
        current+=n;
        return *this;
    }

    self operator-(difference_type n) const
    {
        return self(current+n);
    }

    reference operator[](difference_type n) const
    {
        return *(*this+n);
    }
};

template<class Iterator>
typename reverse_iterator<Iterator>::difference_type
operator- (const reverse_iterator<Iterator>& lhs,const reverse_iterator<Iterator>& rhs)
{
    return rhs.base()-lhs.base();
}

template<class Iterator>
bool operator==(const reverse_iterator<Iterator>& lhs,const reverse_iterator<Iterator>& rhs)
{
    return lhs.base()==rhs.base();
}

template<class Iterator>
bool operator<(const reverse_iterator<Iterator>& lhs,const reverse_iterator<Iterator>& rhs)
{
    return lhs.base()>rhs.base();
}

template<class Iterator>
bool operator!=(const reverse_iterator<Iterator>& lhs,const reverse_iterator<Iterator>& rhs)
{
    return !(lhs==rhs);
}

template<class Iterator>
bool operator>(const reverse_iterator<Iterator>& lhs,const reverse_iterator<Iterator>& rhs)
{
    return lhs.base()<rhs.base();
}

template<class Iterator>
bool operator<=(const reverse_iterator<Iterator>& lhs,const reverse_iterator<Iterator>& rhs)
{
    return !(lhs>rhs);
}

template<class Iterator>
bool operator>=(const reverse_iterator<Iterator>& lhs,const reverse_iterator<Iterator>& rhs)
{
    return !(lhs<rhs);
}

}
#endif