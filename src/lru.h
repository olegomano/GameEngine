#ifndef _LRU_H_
#define _LRU_H_
#include<vector>
#include<unordered_map>

#include"linkedlist.h"

template<typename Key>
class LRU{  
public:
    LRU(){

    }

    void push_back(const Key& k){
        if(!m_map.count(k)){
            auto block = m_list.push_front(k);
            m_map[k]   = block;
        }else{
            auto block = m_map[k];
            m_list.remove(block);
            m_list.push_front(block);
        }
    }

    bool evict(Key& out){
        if(m_list.size() == 0) return false;
        m_list.pop_back(out);
        return true;
    }   

    uint32_t size(){
        return m_map.size();
    }
private:
    std::unordered_map<Key,LinkedList<Key>::Block*> m_map;
    LinkedList<Key>                                 m_list;
};

#endif