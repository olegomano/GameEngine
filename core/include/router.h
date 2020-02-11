#ifndef _ROUTER_H_
#define _ROUTER_H_
#include <stdint.h>
#include <functional>
#include <vector>

#include "info.h"

namespace Router{

template<uint8_t ID,typename T>
struct Rule{
    static constexpr uint8_t id = ID;
    typedef T                type;
};

template<typename... Rules>
class Router{
public:
    static constexpr int MAX_ROUTES = 512;
    static constexpr int MAX_ID     = 256;
public: 
    Router(){
        for(int i = 0; i < MAX_ROUTES; i++){
            m_count[i] = 0;
        }
    }

    template<typename R,typename F>
    void addRoute(const F& f){
        //static_assert(typeid(F::argument_type) == typeid(typename R::type),"Function does not match rule");
        static_assert(tuple_utils::contains<R,Rules...>(), "rule not supported");
        assert(m_count[R::id] < MAX_ROUTES);

        std::function<void(typename R::type)> function = f;
        m_routes[R::id][m_count[R::id]] = new std::function<void(typename R::type)>(function);
        m_count[R::id]++;        
    }

    template<typename R>
    void route(const typename R::type& data){
        static_assert(tuple_utils::contains<R,Rules...>(), "rule not supported");
        for(int i = 0; i < m_count[R::id]; i++){
           std::function<void(typename R::type)>* f = (std::function<void(typename R::type)>*) m_routes[R::id][i];
           (*f)(data);
        }
    }
private:
    void* m_routes[MAX_ID][MAX_ROUTES];
    int   m_count[MAX_ROUTES];
};
};
#endif