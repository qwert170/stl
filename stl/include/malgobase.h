#ifndef MALGOBASE_H_
#define MALGOBASE_H_
/*算法
 *max  min        
 *iter_swap         
 *copy  copy_backward  copy_if copy_n 
 *move  move_backward 
 *equal        
 *fill_n  fill    
 *lexicographical_compare
 *mismatch
 */

#include<cstring>

#include"miterator.h"//迭代器
#include"mutil.h"//move forward pair swap

namespace mystl
{
    #ifdef max
    #pragma message("undefing marco max")
    #undef max
    #endif 

    #ifdef min
    #pragma message("undefing marco min")
    #undef min
    #endif

    //max
    template<class T>
    const T& max(const T& lhs,const T& rhs)
    {
        return lhs<rhs?rhs:lhs;
    }

    //重载版本的
    template<class T,class Compare>
    const T& max(const T& lhs, const T& rhs,Compare comp)
    {
        return comp(lhs,rhs)? rhs:lhs;
    }

    //min
    template<class T>
    const T& min(const T& lhs,const T& rhs)
    {
        return lhs>rhs?rhs:lhs;
    }

    //重载版本的
    template<class T,class Compare>
    const T& max(const T& lhs, const T& rhs,Compare comp)
    {
        return comp(rhs,lhs)? rhs:lhs;
    }

    //迭代器所指对象对掉 iter_swap
    template<class FIter1,class FIter2>
    void iter_swap(FIter1 lhs,FIter2 rhs)
    {
        mystl::swap(*lhs,*rhs);
    }

    //copy   把 [first, last)区间内的元素拷贝到 [result, result + (last - first))内
    //input_iterator_tag
    template<class InputIter,class OutputIter>
    OutputIter unchecked_copy_cat(InputIter first,InputIter last,OutputIter result,mystl::input_iterator_tag)
    {
        for(;first!=last;++first,++result)
        {
            *result=*first;
        }
        return result;
    }

    //random_access_iterator_tag
    //
    template<class RandomIter,class OutputIter>
    OutputIter unchecked_copy_cat(RandomIter first,RandomIter last,OutputIter result,mystl::random_access_iterator_tag)
    {
        for(auto n=last-first;n>0;--n,++first,++result)
        {
            *result=*first;
        }
        return result;
    }

    template<class InputIter,class OutputIter>
    OutputIter unchecked_copy_cat(InputIter frist,InputIter last,OutputIter result)
    {
        return unchecked_copy_cat(frist,last,result,iterator_category(first));
    }

    //is_trivially_copy_assignable具有普通复制赋值运算符的类
    //std::is_same可以判断输入的类型是否是指定的模板类型
    //remove_const<Tp>检查Tp是否没有const限定值。
    //memove从 src 所指向的对象复制 count 个字节到 dest 所指向的对象。两个对象都被转译成 unsigned char 的数组。
    //如同复制字符到临时数组，再从该数组到 dest 一般发生复制。 
    template<class Tp,class Up>
    typename std::enable_if<
      std::is_same<typename std::remove_const<Tp>::type,Up>::value &&
      std::is_trivially_copy_assignable<Up>::value,Up*>::type
    unchecked_copy(Tp* first,Tp* last,Up* result)
    {
        const auto n=static_cast<size_t>(last-first);
        if(n!=0)  std::memmove(result,first,n*sizeof(Up));
        return result+n;
    }

    template<class InputIter,class OutputIter>
    OutputIter copy(InputIter first,InputIter last,OutputIter result)
    {
        return unchecked_copy(first,last,result);
    }

    //copy_backward    将 [first, last)区间内的元素拷贝到 [result - (last - first), result)内
    template<class BidirectionalIter1,class BidirectionalIter2>
    BidirectionalIter2 unchecked_copy_backward_cat(BidirectionalIter1 first,BidirectionalIter1 last,
                                                   BidirectionalIter2 result,mystl::bidirectional_iterator_tag)
    {
        while(first!=last) *--result=*--last;
        return result;
    }

    template<class BidirectionalIter1,class BidirectionalIter2>
    BidirectionalIter2 unchecked_copy_backward_cat(BidirectionalIter1 first,BidirectionalIter1 last,
                                                    BidirectionalIter2 result,mystl::random_access_iterator_tag)
    {
        for(auto n=last-first;n>0;--n) 
            *--result=*--last;
        return result;
    }

    template<class BidirectionalIter1,class BidirectionalIter2>
    BidirectionalIter2 unchecked_copy_backward(BidirectionalIter1 first,BidirectionalIter1 last,
                                                BidirectionalIter2 result)
    {
        return unchecked_copy_backward_cat(first,last,result,iterator_category(first));
    }

    template<class Tp,class Up>
    typename std::enable_if<
      std::is_same<typename std::remove_const<Tp>::type,Up>::value &&
      std::is_trivially_copy_assignable<Up>::value,Up*>::type
    unchecked_copy_backward(Tp* first,Tp* last,Up* result)
    {
        const auto n=static_cast<size_t>(last-first);
        if(n!=0){
            result-=n;
            std::memmove(result,first,n*sizeof(Up));
        }
        return result;
    }

    template<class BidirectionalIter1,class BidirectionalIter2>
    BidirectionalIter2 copy_backward(BidirectionalIter1 first,BidirectionalIter1 last,
                                    BidirectionalIter2 reslut)
    {
        return unchecked_copy_backward(first,last,result);
    }

    //copy_if   把[first, last)内满足一元操作 unary_pred 的元素拷贝到以 result 为起始的位置上
    template<class InputIter,class OutputIter,class UnaryPredicate>
    OutputIter copy_if(InputIter first,InputIter last,OutputIter result,UnaryPredicate unary_pred)
    {
        for(;first!=last;++first)
        {
            if(unary_pred(*first)) *result++=*first;
        }
        return result;
    }

    //copy_n   把 [first, first + n)区间上的元素拷贝到 [result, result + n)上
    // 返回一个 pair 分别指向拷贝结束的尾部
    template<class InputIter,class Size,class OutputIter>
    mystl::pair<InputIter,OutputIter> unchecked_copy_n(InputIter first,Size n,OutputIter result,mystl::input_iterator_tag)
    {
        for(;n>0;--n,+first,++result)
        {
            *result=*first;
        }
        return mystl::pair<InputIter,OutputIter>(first,result);
    }

    template<class InputIter,class Size,class OutputIter>
    mystl::pair<InputIter,OutputIter> unchecked_copy_n(InputIter first,Size n,OutputIter result,mystl::random_access_iterator_tag)
    {
        auto last=first+n;
        return mystl::pair<InputIter,OutputIter>(last,mystl::copy(first,last,result));
    }

    template<class InputIter,class Size,class OutputIter>
    mystl::pair<InputIter,OutputIter> copy_n(InputIter first,Size n,OutputIter result)
    {
        return unchecked_copy_n(first,n,result,iterator_category(first));
    }

    //move  把 [first, last)区间内的元素移动到 [result, result + (last - first))内
    template<class InputIter,class OutputIter>
    OutputIter unchecked_move_cat(InputIter first,InputIter last,
                                  OutputIter result,mystl::input_iterator_tag)
    {
        for(;first!=last;++first,++result)
        {
            *result=mystl::move(*first);
        }
        return result;
    }

    template<class InputIter,class OutputIter>
    OutputIter unchecked_move_cat(InputIter first,InputIter last,
                                 OutputIter result,mystl::random_access_iterator_tag)
    {
        for(auto n=last-first;n>0;--n,++first,++result)
        {
            *result=mystl::move(*first);
        }
        return result;
    }

    template<class InputIter,class OutputIter>
    OutputIter unchecked_move(InputIter first,InputIter last,OutputIter result)
    {
        return unchecked_move_cat(first,last,result,iterator_category(first));
    }

    template<class Tp,class Up>
    typename std::enable_if<
      std::is_same<typename std::remove_const<Tp>::type,Up>::value &&
      std::is_trivially_copy_assignable<Up>::value,Up*>::type
    unchecked_move(Tp* first,Tp* last,Up* result)
    {
        const size_t n=static_cast<size_t>(last-first);
        if(n!=0){
            std::memmove(result,first,n*sizeof(Up));
        }
        return result+n;
    }

    template<class InputIter,class OutputIter>
    OutputIter move(InputIter first,InputIter last,OutputIter result)
    {
        return unchecked_move(first,last,result);
    }

    //move_backward    将 [first, last)区间内的元素移动到 [result - (last - first), result)内
    template<class BidirectionalIter1,class BidirectionalIter2>
    BidirectionalIter2 unchecked_move_backward_cat(BidirectionalIter1 first,BidirectionalIter1 last,
                                                   BidirectionalIter2 result,mystl::bidirectional_iterator_tag)
    {
        while(first!=last)  *--result=mystl::move(*--last);
        return result;
    }

    template<class RandomIter1,class RandomIter2>
    RandomIter2 unchecked_move_backward_cat(BidirectionalIter1 first,BidirectionalIter1 last,
                                                   BidirectionalIter2 result,mystl::random_access_iterator_tag)
    {
       for(auto n=last-first;n>0;n--)  *--result=mystl::move(*--last);
       return result;
    }

    template<class BidirectionalIter1,class BidirectionalIter2>
    BidirectionalIter2 unchecked_move_backward(BidirectionalIter1 first,BidirectionalIter1 last,
                                               BidirectionalIter2 result)
    {
        return unchecked_move_backward_cat(first,last,result,iterator_category(first));
    }

    template<class Tp,class Up>
    typename std::enable_if<
      std::is_same<typename std::remove_const<Tp>::type,Up>::value &&
      std::is_trivially_copy_assignable<Up>::value,Up*>::type
    unchecked_move_backward(Tp* first,Tp* last,Up* result)
    {
        const size_t n=static_cast<size_t>(last-first);
        if(n!=0){
            result-=n;
            std::memmove(result,first,n*sizeof(Up));
        }
        return result;
    }

    template<class BidirectionalIter1,class BidirectionalIter2>
    BidirectionalIter2 move_backward(BidirectionalIter1 first,BidirectionalIter1 last,
                                    BidirectionalIter2 result,)
    {
        return unchecked_move_backward(first,last,result);
    }

    //equal   比较第一序列在 [first, last)区间上的元素值是否和第二序列相等
    template<class InputIter1,class InputIter2>
    bool equal(InputIter1 first1,InputIter1 last1,InputIter2 first2)
    {
        for(;first1!=last1;++first1,++first2)
        {
            if(*first1!=*first2)
            {
                return false;
            }
        }
        return true;
    }

    template<class InputIter1,class InputIter2,class Compared>
    bool equal(InputIter1 first1,InputIter1 last1,InputIter2 first2,Compared comp)
    {
        for(;first!=last1;++first1,++first2)
        {
            if(comp(*first1,*first2))
            {
                return false;
            }
        }
        return true;
    }

    //fill_n    从 first 位置开始填充 n 个值
    template<class OutputIter,class Size,class T>
    OutputIter unchecked_fill_n(OutputIter first,Size n,const T& value)
    {
        for(;n>0;--n,++first)
        {
            *first=value;
        }
        return first;
    }

    //is_integral 检查 T 是否为整数类型 如果 T 是类型 bool ， char ， char8_t ，
    //char16_t ， char32_t ， wchar_t ， short ， int ， long ， long long 
    //或任何实现定义的扩展整数类型，包括任何带符号的类型，则提供等于 true 的成员常 
    //value 无符号且具有简历资格的变体。否则， value 等于 false 。
    template<class Tp,class Size,class Up>
    typename std::enable_if<
        std::is_integral<Tp>::value && sizeof(Tp)==1 &&
        !std::is_same<Tp,bool>::value &&
        std::is_integral<Up>::value && sizeof(Up)==1, Tp*>::type 
    unchecked_fill_n(Tp* first,Size n,Up value)
    {
        if(n>0)
        {
            std::memset(first,(unsigned char)value,(size_t)(n));
        }
        return first+n;
    }

    template<class OutputIter,class Size,class T>
    OutputIter fill_n(OutputIter first,Size n,const T& value)
    {
        return unchecked_fill_n(first,n,value);
    }

    //fill  为 [first, last)区间内的所有元素填充新值
    template<class ForwardIter,class T>
    void fill_cat(ForwardIter first,ForwardIter last,const T& value,
                  mystl::forward_iterator_tag)
    {
        for(;first!=last;++first)
        {
            *first=value;
        }
    }

    template<class RandomIter,class T>
    void fill_cat(RandomIter first,RandomIter last,const T& value,
                  mystl::random_access_iterator_tag)
    {
        fill_n(first,last-first,value);
    }

    template<class ForwardIter,class T>
    void fill(ForwardIter first,ForwardIter last,const T& value)
    {
        fill_cat(first,last,value,iterator_category(first));
    }

    //lexicographical_compare
    // 以字典序排列对两个序列进行比较，当在某个位置发现第一组不相等元素时，有下列几种情况：
    // (1)如果第一序列的元素较小，返回 true ，否则返回 false
    // (2)如果到达 last1 而尚未到达 last2 返回 true
    // (3)如果到达 last2 而尚未到达 last1 返回 false
    // (4)如果同时到达 last1 和 last2 返回 false
    template<class InputIter1,class InputIter2>
    bool lexicographical_compare(InputIter1 first1,InputIter1 last1,
                                InputIter2 first2,InputIter2 last2)
    {
        for(:first1!=last1 && first2!=last2;++first1,++first2)
        {
            if(*first1<*first2) return true;
            if(*first2<*first1) return false;
        }
        return first1==last1 && first2 !=last2;
    }

    template<class InputIter1,class InputIter2,class Compared>
    bool lexicographical_compare(InputIter1 first1,InputIter1 last1,
                                InputIter2 first2,InputIter2 last2,Compared comp)
    {
        for(;first1!=last1 &&first2!=last2;++first1,++first2)
        {
            if(comp(*first1,*first2)) return true;
            if(comp(*first2,*first1)) return false;
        }
        return first1==last1 && first2!=last2;
    }

    //const unsigned char *
    bool lexicographical_compare(const unsigned char * first1,
                                 const unsigned char * last1,
                                 const unsigned char * first2,
                                 const unsigned char * last2)
    {
        const auto len1=last1 - first1;
        const auto len2=last2-first2;
        //比较相同长度的部分 
        const auto result=std::memcmp(first1,first2,mystl::min(len1,len2));
        //比较不同长度 
        return result!=0 ? result<0 :len1<len2;
    }

    //mismatch   平行比较两个序列，找到第一处失配的元素，返回一对迭代器，分别指向两个序列中失配的元素
    template<class InputIter1,class InputIter2>
    mystl::pair<InputIter1,InputIter2>
    mismatch(InputIter1 first1,InputIter1 last1,InputIter2 first2)
    {
        while(first1!=last1 && *first1==*first2)
        {
            ++first1;
            ++first2;
        }
        return mystl::pair<InputIter1,InputIter2>(first1,first2);
    }

    template<class InputIter1,class InputIter2,class Compared>
    mystl::pair<InputIter1,InputIter2>
    mismatch(InputIter1 first1,InputIter1 last1,InputIter2 first2)
    {
        while(first1!=last1 && comp(*first1,*first2))
        {
            ++first1;
            ++first2;
        }
        return mystl::pair<InputIter1,InputIter2>(first1,first2);
    }
} 

#endif