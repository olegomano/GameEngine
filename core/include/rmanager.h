#ifndef _R_MANAGER_H_
#define _R_MANAGER_H_
#include <stdint.h>
#include <vector>
#include <list> 
#include <set>

#include "lru.h"
#include "info.h"

template<typename T>
class RManager;

template<typename ARGS,typename T>
class Resource;

template<typename T>
class RManager{
public:
    friend T;
    static constexpr uint8_t BLOCK_RESERVER_SIZE = 32;
    struct Block{
        static constexpr uint8_t NONE = 0;
        static constexpr uint8_t RAM  = 1<<1;
        
        uint8_t           location = NONE;
        typename T::type* ptr      = nullptr;
        typename T::args  args;
    };
    typedef RManager<T>::Block Block;
public:
    RManager(uint64_t size):m_maxBytes(size){

    }

    T reserve(const typename T::args& args){
        Block* b = getBlock();
        b->location = 0;
        b->ptr      = nullptr;
        b->args     = args;
        T r(b,*this);
        return r;
    }

    T create(const typename  T::args& args){
        T resource = reserve(args);
        init(resource);
        return resource;
    }

private:
    void init(T& resource){
        resource.descriptor->ptr      = allocate<typename T::type,typename T::args>(resource.descriptor->args);
        resource.descriptor->location = Block::RAM;        
        m_bytes+=size<typename T::type>(resource.descriptor->ptr);
        if(m_bytes > m_maxBytes){
            free();
        }
    }

    template<
    typename TYPE,
    typename ARGS>
    TYPE* allocate(const ARGS& args){
        TYPE* t = new TYPE(args);
        return t;
    }

    template<typename TYPE>
    typename std::enable_if<has_size<TYPE>::value,uint32_t>::type
    size(TYPE* t){
        return t->size();
    }

    template<typename TYPE>
    uint32_t size(...){
        return sizeof(TYPE);
    }

    void free(){
        while(m_bytes > m_maxBytes){
            Block* toEvict;
            bool   canEvict = m_lru.evict(toEvict);
            if(!canEvict) return;
            toEvict->location = Block::NONE;
            delete toEvict->ptr;
        }
    }

    Block* getBlock(){
        if(m_freeList.empty()){
            int currSize = m_blockBuffer.size();
            int newSize  = (currSize+8)*2;
            for(int i = currSize; i < newSize; i++){
                Block b;
                m_blockBuffer.push_back(b);
                m_freeList.push_back(&m_blockBuffer.back());
            }
        }
        Block* b = m_freeList.front();
        m_freeList.pop_front();
        return b;
    }

private:
    const uint64_t     m_maxBytes;
    uint64_t           m_bytes     = 0;
    LRU<Block*>        m_lru;
    std::list<Block*>  m_freeList;
};

template<typename T,typename ARGS>
class Resource{
    friend RManager<Resource<T,ARGS>>;
    typedef ARGS args;
    typedef T    type;
    typedef typename RManager<Resource<T,ARGS>>::Block Block;
    typedef RManager<Resource<T,ARGS>> Manager;
public:
    Resource(Block* n, Manager& rm) : manager(rm){
        descriptor = n;
    }

    T* operator ->(){
        manager.m_lru.push_back(descriptor);
        if( !(descriptor->location & Block::RAM) ){
            manager.init(*this);
        }
        return (T*)(descriptor->ptr);
    }

    ARGS params(){
        return descriptor->args;
    }

    ~Resource(){}
private:
    Block*    descriptor = nullptr;
    Manager&  manager;
};


#endif