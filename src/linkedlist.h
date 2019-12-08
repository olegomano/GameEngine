#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_
#include <cinttypes>
#include <iterator>
#include <assert.h>

template<typename T>
class LinkedList{
public:
    static constexpr uint32_t BLOCK_BUFFER_SIZE  = 2056;
    static constexpr uint32_t BLOCK_BUFFER_COUNT = 2056;
    struct Block{
        T      data;
        Block* next;
        Block* prev;
    };

    template<typename IT>
    class Iterator : public std::iterator<std::forward_iterator_tag,IT>{
    public:
        Iterator(Block* start) : block(start){

        }

        Iterator(const Iterator<IT>& o){
            block = o.block;
        }

        Iterator& operator =(const Iterator<IT>& o){
            block = o.block;
            return *this;
        }

        Iterator<IT>& operator++ (){
            block = block->next;
            return *this;
        }

        Iterator<IT> operator++(int){
            Iterator ret(*this);
            block = block->next;
            return ret;
        }

        IT& operator* () const{
            return block->data;
        }

        IT& operator ->() const{
            return block->data;
        }

        bool operator != (const Iterator<IT>& rhs) const{
            return block != rhs.block;
        }

        bool operator == (const Iterator<IT>& rhs) const {
            return block == rhs.block;
        }

         // One way conversion: iterator -> const_iterator
        operator Iterator<const IT>() const{
            return Iterator<const IT>(block);
        }
    private:
        Block* block;
    };

public:
    LinkedList(){
        allocateBlock();   
    }

    Block* push_back(const T& t){
        Block* b = getBlock();
        b->data = t;
        if(m_size == 0){
            m_tail = b;
            m_head = b;
            ++m_size;
            return b;
        }

        insertAfter(b,m_tail);
        m_tail = b;
        ++m_size;
        return b;
    }

    Block* push_front(const T& t){
        Block* b = getBlock();
        b->data = t;
        if(m_size == 0){
            m_tail = b;
            m_head = b;
            ++m_size;
            return b;
        }

        insertBefore(b,m_head);
        m_head = b;
        ++m_size;
        return b;
    }

    void pop_front(){
        Block* oldHead = m_head;
        m_head = m_head->next;
        removeBlock(oldHead);
        --m_size;
    }

    void pop_front(T& dst){
        dst = m_head->data;
        Block* oldHead = m_head;
        m_head = m_head->next;
        removeBlock(oldHead);
        --m_size;
    }

    void pop_back(){
        Block* oldTail = m_tail;
        m_tail = m_tail->prev;
        removeBlock(oldTail);
        --m_size;
    }

    void pop_back(T& dst){
        dst    = m_tail->data;
        Block* oldTail = m_tail;
        m_tail = m_tail->prev;
        removeBlock(oldTail);
        --m_size;
    }
    
    void remove(Block* b){
        removeBlock(b);
    }

    uint32_t size() const{
        return m_size;
    }

    static constexpr uint32_t capcity()  {
        return BLOCK_BUFFER_SIZE*BLOCK_BUFFER_COUNT;
    }

    Iterator<T> begin(){
        return Iterator<T>(m_head);
    }

    Iterator<T> end(){
        return Iterator<T>(m_tail);
    }

    ~LinkedList(){
        for(uint32_t i = 0; i < m_blockCount; i++){
            delete[] m_blockBuffers[i];
        }
    }
private:
    Block* getBlock(){
        if(m_freeBlocks == nullptr){
            allocateBlock();
        }
        Block* b     = m_freeBlocks;
        m_freeBlocks = b->next; 
        b->next      = nullptr;
        b->prev      = nullptr;
        return b;
    }

    void allocateBlock(){
        assert(m_blockCount < BLOCK_BUFFER_COUNT);
        
        m_blockBuffers[m_blockCount] = new Block[BLOCK_BUFFER_SIZE];   
        
        m_freeBlocks       = m_blockBuffers[m_blockCount];
        m_freeBlocks->next = nullptr;
        m_freeBlocks->prev = nullptr;
        for(uint32_t i = 1; i < BLOCK_BUFFER_SIZE; i++){
            Block* b = m_blockBuffers[m_blockCount] + i;
            insertBefore(b,m_freeBlocks);
            m_freeBlocks = b;
        }
        ++m_blockCount;
    }

    void insertAfter(Block* node, Block* list){
        node->prev = list;
        node->next = list->next;
        list->next = node;
        if(node->next != nullptr){
            node->next->prev = node;
        }
    }

    void insertBefore(Block* node, Block* list){
        node->next = list;
        node->prev = list->prev;
        list->prev = node;
        if(node->prev != nullptr){
            node->prev->next = node;
        }
    }

    void removeBlock(Block* node){
        if(node->next != nullptr){
            node->next->prev = node->prev;
        }
        if(node->prev != nullptr){
            node->prev->next = node->next;
        }
        node->next = nullptr;
        node->prev = nullptr;
        if(m_freeBlocks == nullptr){
            m_freeBlocks = node;
        }else{
            insertAfter(node,m_freeBlocks);
        }
    }


private:
    Block*   m_blockBuffers[BLOCK_BUFFER_COUNT];
    uint32_t m_blockCount  = 0;
    uint32_t m_size        = 0;

    Block*   m_head       = nullptr;
    Block*   m_tail       = nullptr;
    Block*   m_freeBlocks = nullptr;
};


#endif