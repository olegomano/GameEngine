#ifndef _CORE_EVENTBUS_
#define _CORE_EVENTBUS_

#include <atomic>
#include <vector>
#include <string>
#include <memory.h>
#include <functional>
#include <map>

namespace core{
namespace eventbus{

template<uint64_t _T_Size>
class Block{
public:
  static constexpr uint64_t BUFFER_SIZE = _T_Size;
  static constexpr uint64_t ERR_FULL = -1;
  template<typename T>
  uint64_t write(const T& t){
    uint64_t r = m_ptr;
    if(r + sizeof(T) > _T_Size){
      return ERR_FULL;
    }
    memcpy(m_buffer+m_ptr,(uint8_t*)(&t),sizeof(T));
    m_ptr+=sizeof(T);
    return r;
  }

  void clear(){
    m_ptr = 0;
  }

  Block<_T_Size>* next(){
    if(m_next == nullptr){
      m_next = new Block<_T_Size>();
    }
    return m_next;
  }
  
  uint8_t* operator[] (uint64_t t){
    return (uint8_t*)(&m_buffer[t]);
  }

  template<typename T>
  T* get(uint64_t ptr){
    uint8_t* curr = &m_buffer[ptr];
    return (T*)curr;
  }

private: 
  uint8_t  m_buffer[_T_Size];
  uint64_t m_ptr = 0;
  Block<_T_Size>* m_next = nullptr;
};

template<typename T>
struct Event{
  uint64_t id;
  T         value;
};


typedef std::function<void(uint64_t,void*)> Listener;
/**
 * flush and send cant happen at the same time
 **/
template<typename _T_Events>
class EventBus{
public:
  typedef Block<2048> EventBlock;
  
  void addListener(Listener l){
    m_listeners.push_back(l);
  }

  template<typename T>
  void addFilteredListenre(Listener l){
    constexpr uint64_t id = _T_Events::template type_id<T>::value;
    
    if(m_filteredListeners.find(id) == m_filteredListeners.end()){
      m_filteredListeners[id] = std::vector<Listener>(16);
    }
    m_filteredListeners[id].push_back(l);
  }

  template<typename T>
  void send(const T& event){
    constexpr uint64_t id  = _T_Events::template type_id<T>::value;
    static_assert(sizeof(Event<T>) < EventBlock::BUFFER_SIZE);
    
    Event<T> e   = {id,event};
    uint64_t ptr = m_tail->write(e);
    
    if(ptr == EventBlock::ERR_FULL){
      m_tail = m_tail->next();
      send(event);
    }else{
      m_eventPtrs.push_back(ptr);
    }
  }


  void flush(){
    EventBlock* iterator  = m_head;
    uint64_t    lastPtr   = -1;

    for(uint64_t i = 0; i < m_eventPtrs.size(); i++){
      uint64_t ptr = m_eventPtrs[i];
      if(ptr <= lastPtr && lastPtr != -1){
        iterator->clear();
        iterator = iterator->next();  
      }
      Event<void*>* evt = iterator->get<Event<void*>>(ptr);

      if(m_filteredListeners.find(evt->id) == m_filteredListeners.end()){
        for(Listener l : m_filteredListeners[evt->id]){
          l(evt->id,&evt->value); 
        }
      }
      for(Listener l : m_listeners){
        l(evt->id,&evt->value); 
      }
      lastPtr = ptr;
    }
    iterator->clear();
    m_eventPtrs.clear();
    m_tail = m_head;
  }
  
private:
  std::vector<uint64_t> m_eventPtrs;
  EventBlock*    m_head = new EventBlock();
  EventBlock*    m_tail = m_head;
  
  std::vector<Listener> m_listeners;
  std::map<uint64_t,std::vector<Listener>> m_filteredListeners;
};



}
}






#endif
