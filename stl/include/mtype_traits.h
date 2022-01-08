#ifndef MTYPE_TRAITS_H_
#define MTYPE_TRAITS_H_

#include<type_traits>
//判断类型 定义了真假
namespace mystl{

template<class T,T v>//T v 使T的类型被推导出来
struct m_integral_constant
{
    static constexpr T value=v;
};

template<bool B>//辅助别名模板 类型别名
using m_bool_constant=m_integral_constant<bool,B>;

typedef m_bool_constant<true> m_true_type;
typedef m_bool_constant<false> m_false_type;

//pair
template<class T1,class T2>
struct pair;

template<class T>
struct is_pair:mystl::m_false_type{};//

template<class T1,class T2>
struct is_pair<mystl::pair<T1,T2> > :mystl::m_true_type{};//
}

#endif