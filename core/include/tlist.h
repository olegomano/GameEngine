#ifndef _CORE_TLIST_
#define _CORE_TLIST_
#include <tuple>
#include <variant>


namespace core{
template<typename ..._T_List>
class TList{
private:
  
  template<typename T>
  struct has_type;

  template<typename T>
  struct has_type<T> : std::disjunction<std::is_same<T,_T_List>...>{};
  
  template<typename T, size_t index = 0>
  constexpr size_t union_index(){
    if(index == std::tuple_size<std::tuple<_T_List...>::value){
      return -1;
    }
    else if constexpr ( std::is_same<T,std::tuple_element<index,std::tuple<_T_List...>>::type>::value  ){
      return index;
    }
    return union_index<T,index + 1>()
  }

public:
  typedef std::variant<_T_List...> Union;
  typedef std::tuple<_T_List...>   List;
 
  template<typename T>
  constexpr size_t IndexOf(){
    return union_index<T>(); 
  }

  template<typename T>
  constexpr bool Contains(){
    
  }
};

}
#endif
