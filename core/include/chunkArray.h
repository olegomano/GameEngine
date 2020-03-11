#ifndef _CORE_CHUNK_ARRAY_
#define _CORE_CHUNK_ARRAY_

#include <vector>
#include <cinttypes>
#include <iostream>
namespace core{
namespace block_array{


template<uint32_t SIZE>
struct Block{
  uint8_t  data[SIZE];
  uint32_t count = 0;
};


template<typename T>
class BlockArray{
public:
  static constexpr uint32_t BlockCount = 1024;
  static constexpr uint32_t BlockSize  = BlockCount * sizeof(T);

  void push_back(const T& t){
    uint32_t block = m_size / BlockCount;
    if(block >= m_blocks.size()){
      m_blocks.push_back(new Block<BlockSize>());
    }
    Block<BlockSize>* c = m_blocks[block];
    uint32_t ptr = c->count * sizeof(T);
    new ( c->data + ptr) T(t);
    c->count++;
    m_size++;
    
  }

  size_t size() const {
    return m_size;
  }

  void reserve(uint32_t t){
    m_blocks.reserve(t);
  }

  T& operator[](const uint32_t t){
    Block<BlockSize>* c = m_blocks[t/BlockSize];
    uint32_t ptr = t % BlockCount;
    T*      tptr = (T*)c->data + ptr;
    return *tptr;
  }
private:
  std::vector<Block<BlockSize>*>  m_blocks;
  uint32_t                        m_size = 0;
};


};
};
#endif

