#ifndef _TYPE_INFO_H_
#define _TYPE_INFO_H_


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



#endif