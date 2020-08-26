#ifndef _CORE_WORKQUE_
#define _CORE_WORKQUE_
#include <vector>
#include <mutex>
#include <cinttypes>
#include <atomic>
#include <array>

namespace core{
namespace workque{

template<typename T>
struct Task{
  T        runnable;
  Task<T>* next = nullptr;
};


template<typename T,uint32_t SIZE>
struct TaskBlock{
  std::atomic<uint32_t> count = 0;
  T                     tasks[SIZE];
  TaskBlock<T,SIZE>*    next = nullptr;
  TaskBlock<T,SIZE>*    prev = nullptr;
};

template<typename T, uint32_t SIZE>
struct TaskList{
  typedef TaskBlock<T,SIZE> Block;
  Block* head = nullptr;
  Block* tail = nullptr;
  Block* free = nullptr;
  std::mutex         allocationLock;

  TaskList(){
    head = new Block();
    tail = new Block();
    head->next = tail;
    head->prev = nullptr;

    tail->next = nullptr;
    tail->prev = head;
  }

  void push_back(const T& data){
    Block*   b    = tail;
    uint32_t indx = b->count++;
    while(indx>=SIZE){
      allocationLock.lock();
      b    = tail;
      indx = b->count++;
      if(indx>=SIZE){
        allocationLock.unlock();
        continue;
      }

      if(free != nullptr){
        b = free;
        free = b->next;
        b->next = nullptr;
      }else{
        b = new Block();
      }

      indx = b->count++;
      tail->next = b;
      b->prev    = tail;
      tail       = b;
      allocationLock.unlock();
    }
  }

};

template<typename T>
class WorkQue{
public:
  static constexpr uint32_t BlockSize = 1024;
  typedef TaskList<T,BlockSize> Tasks;
  
  WorkQue(){
  
  }


  void schedule(const T& func){
    Tasks* writeList = m_writeList;
    writeList->push_back(func);
  }
  
  uint32_t runAll(){
    Tasks* toRun = m_writeList.exchange(m_runList);
    uint32_t taskCount = 0;
    
    typename Tasks::Block* b = toRun->head;
    while(b != nullptr){
      for(int i =0; i < b->count; i++){
        b->tasks[i].runnable();
        ++taskCount;
      }
      b->count = 0;
      typename Tasks::Block* processedBlock = b;
      b = b->next;

      processedBlock->next = toRun->free;
      toRun->free = processedBlock;
    }

    return taskCount;
  }

private:
  Tasks   m_taskLists[2];
  std::atomic<Tasks*>  m_writeList  = &m_taskLists[0];
  std::atomic<Tasks*>  m_runList    = &m_taskLists[1]; 
};



};
};
#endif
