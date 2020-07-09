#ifndef _EVENT_QUE_H_
#define _EVENT_QUE_H_
#include <iostream>
#include <stdint.h>
#include <stddef.h>
#include <type_traits>
#include <atomic>
#include <mutex>
#include <deque>
namespace core{
namespace task_que{

template<typename T>
struct _Task{
  const uint32_t id;
  const T task;
  
  _Task(uint32_t i,T t) : id(i),task(t){}
    
  _Task(const _Task<T>& task) : id(task.id),task(task.task)
  {}
};

template<typename T>
class TaskQue{
public:
  typedef _Task<T> Task;
  template<typename F>
  bool push_back(F&& f){
    Task t = Task(m_taskId++,f);
    {
      std::unique_lock<std::mutex> lock(m_lock);
      m_deque.push_back(t);
    }
    ++m_taskCount;
    return true;
  }

  uint32_t run(){
    uint32_t taskCount = 0;
    uint32_t maxId = m_taskId;
    if(m_taskCount == 0) return 0;
    while(m_taskCount > 0){
      m_lock.lock();
      Task task = m_deque.front();

      if(task.id < maxId){
        m_deque.pop_front();
        m_lock.unlock();

        task.task();
        ++taskCount;
        --m_taskCount;
      }else{
        m_lock.unlock();
        break;
      }
    }
    return taskCount;
  }

private:
  std::atomic<uint32_t> m_taskCount = 0;
  std::atomic<uint32_t> m_taskId    = 0;
  std::deque<Task> m_deque;
  std::mutex m_lock;
};

};
};


#endif
