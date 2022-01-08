#ifndef MUNINITIALIZED_H_
#define MUNINITIALIZED_H_

/*对未初始化空间构造元素
 *uninitialized_copy   uninitialized_copy_n 
 *uninitialized_fill   uninitialized_fill_n
 *uninitialized_move   uninitialized_move_n 
 */
#include "malgobase.h"
#include "mconstruct.h"
#include "miterator.h"
#include "mtype_traits.h"
#include "mutil.h"

namespace mystl
{
    //uninitialized_copy  把 [first, last) 上的内容复制到以 result 为起始处的空间，返回复制结束的位置

    template<class InputIter,class ForwardIter>
    ForwardIter unchecked_uninit_copy(InputIter first,InputIter last,ForwardIter result,mystl::m_true_type)
    {
        return mystl::copy(first,last,result);
    }

    template<class InputIter,class ForwardIter>
    ForwardIter unchecked_uninit_copy(InputIter first,InputIter last,ForwardIter result,mystl::m_false_type)
    {
        auto cur=result;
        try
        {
            for(;first!=last;++first,++cur)
            {
                mystl::construct(&*cur,*first);
            }
        }
        catch(...)//捕获所有错误
        {
            for(;result!=cur;--cur)
                mystl::destory(&*cur);
        }
        return cur;
    }

    //is_trivially_copy_assignable具有普通复制赋值运算符的类
    template<class InputIter,class ForwardIter>
    ForwardIter uninitialized_copy(InputIter first,InputIter last,ForwardIter result)
    {
        return mystl::unchecked_uninit_copy(first,last,result,
                                            std::is_trivially_copy_assignable<typename iterator_traits<ForwardIter>::value_type{});                             
    }

    // uninitialized_copy_n   把 [first, first + n) 上的内容复制到以 result 为起始处的空间，返回复制结束的位置

    template<class InputIter,class Size,class ForwardIter>
    ForwardIter unchecked_uninit_copy_n(InputIter first,Size n ,ForwardIter result,mystl::m_true_type)
    {
        return mystl::copy_n(first,n,result).second;
    }

    template<class InputIter,class Size,class ForwardIter>
    ForwardIter unchecked_uninit_copy_n(InputIter first,Size n,ForwardIter result,mystl::m_false_type)
    {
        auto cur=result;
        try
        {
            for(;n>0;n--,++first,++cur)
            {
                mystl::construct(&*cur,*first);
            }
        }
        catch(...)//捕获所有错误
        {
            for(;result!=cur;--cur)
                mystl::destory(&*cur);
        }
        return cur;
    }

    //is_trivially_copy_assignable具有普通复制赋值运算符的类
    template<class InputIter,class Size,class ForwardIter>
    ForwardIter uninitialized_copy_n(InputIter first,Size n,ForwardIter result)
    {
        return mystl::unchecked_uninit_copy_n(first,n,result,
                                            std::is_trivially_copy_assignable<typename iterator_traits<InputIter>::value_type{});                             
    }


    //uninitialized_fill   在 [first, last) 区间内填充元素值

    template<class InputIter,class T>
    void unchecked_uninit_fill(InputIter first,InputIter last,const T& value,mystl::m_true_type)
    {
        return mystl::fill(first,last,value);
    }

    template<class InputIter,class T>
    void unchecked_uninit_fill(InputIter first,InputIter last,const T& value,mystl::m_false_type)
    {
        auto cur=first;
        try
        {
            for(;cur!=last;++cur)
            {
                mystl::construct(&*cur,value);
            }
        }
        catch(...)//捕获所有错误
        {
            for(;first!=cur;++first)
                mystl::destory(&*first);
        }
    }

    //is_trivially_copy_assignable具有普通复制赋值运算符的类
    template<class InputIter,class T>
    void uninitialized_fill(InputIter first,InputIter last,const T& value)
    {
        return mystl::unchecked_uninit_fill(first,last,value,
                                            std::is_trivially_copy_assignable<typename iterator_traits<InputIter>::value_type{});                             
    }

    //uninitialized_fill_n   从 first 位置开始，填充 n 个元素值，返回填充结束的位置

    template<class InputIter,class Size,class T>
    InputIter unchecked_uninit_fill_n(InputIter first,Size n,const T& value,mystl::m_true_type)
    {
        return mystl::fill_n(first,n,value);
    }

    template<class InputIter,class Size,class T>
    InputIter unchecked_uninit_fill(InputIter first,Size n,const T& value,mystl::m_false_type)
    {
        auto cur=first;
        try
        {
            for(;n>0;--n,++cur)
            {
                mystl::construct(&*cur,value);
            }
        }
        catch(...)//捕获所有错误
        {
            for(;first!=cur;++first)
                mystl::destory(&*first);
        }
        return cur;
    }

    //is_trivially_copy_assignable具有普通复制赋值运算符的类
    template<class InputIter,class Size,class T>
    void uninitialized_fill_n(InputIter first,Size n,const T& value)
    {
        return mystl::unchecked_uninit_fill_n(first,n,value,
                                            std::is_trivially_copy_assignable<typename iterator_traits<InputIter>::value_type{});                             
    }

    //uninitialized_move 把[first, last)上的内容移动到以 result 为起始处的空间，返回移动结束的位置

    template<class InputIter,class ForwardIter>
    ForwardIter unchecked_uninit_move(InputIter first,InputIter last,ForwardIter result,mystl::m_true_type)
    {
        return mystl::move(first,last,result);
    }

    template<class InputIter,class ForwardIter>
    ForwardIter unchecked_uninit_move(InputIter first,InputIter last,ForwardIter result,mystl::m_false_type)
    {
        ForwardIter cur=result;
        try
        {
            for(;first!=last;++first,++cur)
            {
                mystl::construct(&*cur,mystl::move(*first));
            }
        }
        catch(...)//捕获所有错误
        {
            mystl::destory(result,cur);
        }
        return cur;
    }

    //is_trivially_copy_assignable具有普通复制赋值运算符的类
    template<class InputIter,class ForwardIter>
    ForwardIter uninitialized_move(InputIter first,InputIter last,ForwardIter result)
    {
        return mystl::unchecked_uninit_move(first,last,result,
                                            std::is_trivially_copy_assignable<typename iterator_traits<InputIter>::value_type{});                             
    }

    //uninitialized_move_n  把[first, first + n)上的内容移动到以 result 为起始处的空间，返回移动结束的位置

    template<class InputIter,class Size,class ForwardIter>
    ForwardIter unchecked_uninit_move_n(InputIter first,Size n,ForwardIter result,mystl::m_true_type)
    {
        return mystl::move(first,first+n,result);
    }

    template<class InputIter,class Size,class ForwardIter>
    ForwardIter unchecked_uninit_move_n(InputIter first,Size n,ForwardIter result,mystl::m_false_type)
    {
        ForwardIter cur=result;
        try
        {
            for(;n>0;n--,++first,++cur)
            {
                mystl::construct(&*cur,mystl::move(*first));
            }
        }
        catch(...)//捕获所有错误
        {   
            for(;result!=cur;++result)
                mystl::destory(&*result);
            throw;
        }
        return cur;
    }

    //is_trivially_copy_assignable具有普通复制赋值运算符的类
    template<class InputIter,class ForwardIter>
    ForwardIter uninitialized_move(InputIter first,InputIter last,ForwardIter result)
    {
        return mystl::unchecked_uninit_move(first,last,result,
                                            std::is_trivially_copy_assignable<typename iterator_traits<InputIter>::value_type{});                             
    }
}

#endif