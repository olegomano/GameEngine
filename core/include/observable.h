#ifndef _OBSERVABLE_H_
#define _OBSERVABLE_H_
#include <vector>
#include <functional>

template <typename T>
class Observable{
public:
    template<typename F>
    auto observe(const F& function) const {
        m_observers.push_back(function);
        return function;
    }

    Observable& operator=(const T& data){
        m_value = data;
    }

    void operator()(const T& data){
        setAndBroadcast(data);
    }

    T* operator ->(){
        return &m_value;    
    }

    const T* const operator->() const {
        return &m_value;
    }
    
    void setAndBroadcast(const T& data){
        m_value = data;
        broadcast();
    }

    void broadcast(){
        for(auto f : m_observers){
            f(m_value);
        }
    }
protected:
    std::vector<std::function<void(const T& data)>> mutable m_observers;
    T                                                       m_value;
};


template<typename T>
class Mutator : Observable<T>{
public:
    Mutator(std::function<void(const T& data,T& result)>& functor) : m_function(functor){

    }

    void operator()(const T& data){
        m_function(data,((Observable<T>*)this)->m_value);
        ((Observable<T>*)this)->broadcast();
    }

private:
    const std::function<void(const T& data, T& result)> m_function;
};

#endif