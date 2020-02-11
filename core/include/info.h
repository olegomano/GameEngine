#ifndef _TYPE_INFO_H_
#define _TYPE_INFO_H_
#include <type_traits>

template<typename T>
struct has_size {

    template<typename U>
    static constexpr decltype(std::declval<U>().size(),bool())
    test_hasSize(int){
        return true;
    };

    template<typename U>
    static constexpr bool test_hasSize(...){
        return false;
    };

    static constexpr bool value = test_hasSize<T>(int());
};

namespace tuple_utils{
    
    /**
    namespace internal{
    
    template<int... Is>
    struct seq { };

    template<int N, int... Is>
    struct gen_seq : gen_seq<N - 1, N - 1, Is...> { };

    template<int... Is>
    struct gen_seq<0, Is...> : seq<Is...> { };

    template<typename T, typename F, int... Is>
    void for_each(T&& t, F f, seq<Is...>){
        auto l = { (f(std::get<Is>(t)), 0)... };
    };

    template<typename T, typename Args, int... Is>
    void invoke(T&& t, Args args,seq<Is...>){
        std::get<Is>(t)(args);
    };  

    };

    template<typename... Ts, typename F>
    void for_each(std::tuple<Ts...> const& t, F f){
        internal::for_each(t, f, internal::gen_seq<sizeof...(Ts)>());
    }

    template<typename... Ts, typename Args>
    void invoke(std::tuple<Ts...> const& t, const Args& args){
        internal::invoke(t,args,internal::gen_seq<sizeof...(Ts)>())
    }
    **/
    template<typename T, typename... Ts>
    constexpr bool contains(){ 
        return std::disjunction_v<std::is_same<T, Ts>...>; 
    }


};



#endif