#ifndef _CORE_LATCH_
#define _CORE_LATCH_

#include <atomic>
#include <memory>

namespace core{

class Latch{
public:
  void open(){
    m_open->store(true);
  }

  void close(){
    m_open->store(false);
  }

  void pass(){
    while(!m_open->load()){}
  }

private:
  std::shared_ptr<std::atomic<bool>> m_open = std::make_shared<std::atomic<bool>>();
};

}
#endif
