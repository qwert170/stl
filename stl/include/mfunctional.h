#ifndef MFUNCTIONAL_H_
#define MFUNCTIONAL_H_

//函数对象和哈希函数  仿函数
#include <cstddef>

namespace mystl
{
    template<class Arg,class Result>
    struct unarg_function
    {
        typedef Arg argument_type;
        typedef Result result_type;
    };

    template<class Arg1,class Arg2,class Result>
    struct binary_function
    {
        typedef Arg1 first_argument_type;
        typedef Arg2 second_argument_type;
        typedef Result result_type;
    };

    template<class T>
    struct plus:public binary_function<T,T,T>
    {
        T operator()(const T& x,const T& y)const{return x+y;}
    };

    template<class T>
    struct minus:public binary_function<T,T,T>
    {
        T operator()(const T& x,const T& y)const{return x-y;}
    };

    template<class T>
    struct multiplied:public binary_function<T,T,T>
    {
        T operator()(const T& x,const T& y)const{return x*y;}
    };

    template<class T>
    struct divides:public binary_function<T,T,T>
    {
        T operator()(const T& x,const T& y)const{return x/y;}
    };

    template<class T>
    struct modulus:public binary_function<T,T,T>
    {
      T operator()(const T& x,const T& y)const{return x%y;}  
    };
    
    //否定
    template<class T>
    struct negate:public unarg_function<T,T>
    {
        T operator()(const T& x)const{return -x;}
    };
    
    /*证同元素(identity element)

     *数值A若与该元素做op运算，会得到A自己；
     *加法的证同元素为0
     *乘法的证同元素为1
     */
    //加法的证同元素
    template<class T>
    T identity_element(plus<T>){return T(0);}

    template<class T>
    T identity_element(multiplied<T>){return T(1);}

    template<class T>
    struct equal_to:public binary_function<T,T,bool>
    {
        bool operator()(const T& x,const T& y)const{return x==y;}
    };

    template<class T>
    struct not_equal_to:public binary_function<T,T,bool>
    {
        bool operator()(const T& x,const T& y)const{return x!=y;}
    };

    template<class T>
    struct greater:public binary_function<T,T,bool>
    {
        bool operator()(const T& x,const T& y)const{return x>y;}
    };

    template<class T>
    struct less:public binary_function<T,T,bool>
    {
        bool operator()(const T& x,const T& y)const{return x<y;}
    };
    
    template<class T>
    struct greater_equal:public binary_function<T,T,bool>
    {
        bool operator()(const T& x,const T& y)const{return x>=y;}
    };

    template<class T>
    struct less_equal:public binary_function<T,T,bool>
    {
        bool operator()(const T& x,const T& y)const{return x<=y;}
    };

    template<class T>
    struct logical_and:public binary_function<T,T,bool>
    {
        bool operator()(const T& x,const T& y)const{return x&&y;}
    };

    template<class T>
    struct logical_or:public binary_function<T,T,bool>
    {
        bool operator()(const T& x,const T& y)const{return x||y;}
    };

    template<class T>
    struct logical_not:public unarg_function<T,bool>
    {
        bool operator()(const T& x)const{return !x;}
    };

    //证同元素：不会改变元素，返回本身
    template<class T>
    struct identity:public unarg_function<T,bool>//T
    {
        const T& operator()(const T& x)const{return x;}
    };

    //选择函数
    template<class Pair>
    struct selectfirst:public unarg_function<Pair,typename Pair::first_type>
    {
        const typename Pair::first_type& operator()(const Pair& x)const{return x.first;}
    };

    template<class Pair>
    struct selectsecond:public unarg_function<Pair,typename Pair::second_type>
    {
        const typename Pair::second_type& operator()(const Pair& x)const{return x.second;}
    };

    //投射函数
    template<class Arg1,class Arg2>
    struct projectfirst:public binary_function<Arg1,Arg2,Arg1>
    {
        Arg1 operator()(const Arg1& x,const Arg2&)const{return x;}
    };

    template<class Arg1,class Arg2>
    struct projectsecond:public binary_function<Arg1,Arg2,Arg2>
    {
        Arg2 operator()(const Arg1&,const Arg2& x)const{return x;}
    };


    


    //哈希函数
    // 对于大部分类型，hash function 什么都不做
    template<class Key>
    struct hash{};

    template<class T>
    struct hash<T*>
    {
        size_t operator()(T* p)const noexcept
        {return reinterpret_cast<size_t>(p);}
    };

    #define MYSTL_TRIVIAL_HASH_FCN(Type)            \
    template<> struct hash<Type>                    \
    {                                               \
        size_t operator()(Type val)const noexcept   \
        {return static_cast<size_t>(val);}          \
    };

    MYSTL_TRIVIAL_HASH_FCN(bool)

    MYSTL_TRIVIAL_HASH_FCN(char)

    MYSTL_TRIVIAL_HASH_FCN(signed char)

    MYSTL_TRIVIAL_HASH_FCN(unsigned char)

    MYSTL_TRIVIAL_HASH_FCN(wchar_t)

    MYSTL_TRIVIAL_HASH_FCN(char16_t)

    MYSTL_TRIVIAL_HASH_FCN(char32_t)

    MYSTL_TRIVIAL_HASH_FCN(short)

    MYSTL_TRIVIAL_HASH_FCN(unsigned short)

    MYSTL_TRIVIAL_HASH_FCN(int)

    MYSTL_TRIVIAL_HASH_FCN(unsigned int)

    MYSTL_TRIVIAL_HASH_FCN(long)

    MYSTL_TRIVIAL_HASH_FCN(unsigned long)

    MYSTL_TRIVIAL_HASH_FCN(long long)

    MYSTL_TRIVIAL_HASH_FCN(unsigned long long)

    #undef MYSTL_TRIVIAL_HASH_FCN

    //fnv_offset FNV偏移    fnv_prime  FNV素值
    inline size_t bitwise_hash(const unsigned char * first,size_t count)
    {
    #if (_MSC_VER && _WIN64) || ((__GUNC__ || __clang__) &&__SIZEOF_POINTER__==8 )
        const size_t fnv_offset =14695981039346656037ull;//该FNV_offset_basis是64位 FNV偏移基础值：14695981039346656037
        const size_t fnv_prime = 1099511628211ull;
    #else
        const size_t fnv_offset = 2166136261u;
        const size_t fnv_prime = 16777619u;
    #endif
        size_t result=fnv_offset;
        for(size_t i=0;i<count;++i)
        {
            result ^=(size_t)first[i];//^= 按位异或后赋值
            result*=fnv_prime;
        }
        return result;
    }

    template<>
    struct hash<float>
    {
        size_t operator()(const float& val)
        {
            return val==0.0f?0:bitwise_hash((const unsigned char*)&val,sizeof(float));
        }
    };

    template<>
    struct hash<double>
    {
        size_t operator()(const double& val)
        {
            return val==0.0f ?0:bitwise_hash((const unsigned char*)&val,sizeof(double));
        }
    };

    template<>
    struct hash<long double>
    {
        size_t operator()(const long double& val)
        {
            return val==0.0f ?0:bitwise_hash((const unsigned char *)&val,sizeof(long double));
        }
    };

}
#endif