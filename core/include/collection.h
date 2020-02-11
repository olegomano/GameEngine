#ifndef _COLLECTION_H_
#define _COLLECTION_H_

#include <assert.h>
#include <bits/stdc++.h> 
#include <vector>
#include "linkedlist.h"


template<typename T>
class Collection{
public:
    class Ptr{
    friend Collection<T>;
    public:
        T* operator->(){
            return &m_owner[m_index->data];
        }

        T& operator*(){
            return m_owner[m_index->data];
        }
    

        Ptr& operator=(const T& t){
            m_owner[m_index->data] = t;
            return *this;
        }
    private:
        Ptr(Collection<T>& owner, LinkedList<uint32_t>::Block* b)
        :
        m_index(b),
        m_owner(owner)
        {}
        

        LinkedList<uint32_t>::Block* m_index;
        Collection<T>&               m_owner;
    };
public:
    template<typename ...ARGS>
    Ptr allocate(const ARGS& ... args){
        Entry e;
        e.data  = T(args...);
        e.index = m_indexes.push_back(m_data.size());
        m_data.push_back(e);
        return Ptr(*this,e.index);
    }


    T& operator[](uint32_t indx) {
        return m_data[indx].data;
    }

    uint32_t size() const{
        return m_data.size();
    }

    void erase(uint32_t indx){
        swap(m_data.size() - 1,indx);
        m_data.erase(m_data.size() - 1);
    }        

    void erase(Ptr ptr){
        erase(ptr.m_index->data);
    }
private:
    struct Entry{
        LinkedList<uint32_t>::Block* index;
        T                            data;
    };
private:
    void swap(uint32_t a, uint32_t b){
        Entry& ae = m_data[a];
        Entry& be = m_data[b];

        ae.index->data = b;
        be.index->data = a;
        std::swap(m_data[a],m_data[b]);
    }
private:
    LinkedList<uint32_t> m_indexes;
    std::vector<Entry>   m_data;
    
};






#endif