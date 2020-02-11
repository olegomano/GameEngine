#ifndef _V_STACK_H_
#define _V_STACK_H_
#include <inttypes.h>
#include <assert.h>

template<uint32_t SIZE>
class StaticStack{

    template<typename T>
    T* allocate(){
        assert(m_ptr + sizeof(T) < SIZE);
        void* result = &m_data[m_ptr];
        m_ptr += sizeof(T);
        return new(result) T;
    }

    void free(){
        m_ptr = 0;
    }

private:
    uint8_t   m_data[SIZE];
    uint32_t  m_ptr = 0;
};

class FixedStack{
public:
    FixedStack(uint32_t size) : 
    m_data(new uint8_t[size]),
    m_size(size)
    {
        m_ptr = 0;
    }

    template<typename T>
    T* allocate(){
        assert(m_ptr + sizeof(T) < m_size);
        void* result = (m_data + m_ptr);
        m_ptr += sizeof(T);
        return new(result) T;
    }

    void free(){
        m_ptr = 0;
    }

    ~FixedStack(){
        delete[] m_data;
    }
private:
    const uint32_t m_size;
    uint8_t* const m_data;
    uint32_t       m_ptr;
};

class DynamicStack{
private:
    struct Block{
        static constexpr uint32_t SIZE = 1024;
        uint8_t  buffer[SIZE];
        uint16_t ptr  = 0;
        Block*   next = nullptr;
    };

public:
    DynamicStack(uint32_t initialSize = Block::SIZE){
        m_head = new Block();
        m_tail = m_head;
        int initialBlocks = (initialSize / Block::SIZE) - 1;
        if(initialBlocks > 0){
            for(int i = 0; i < initialBlocks; i++){
                addBlock(new Block());
            }
        }
        free();
    }

    void free(){
        Block* b = m_head;
        do{
            b->ptr = 0;
            b = b->next;
        }while(b != nullptr);

        if(m_free != nullptr){
            insertNext(m_free,m_head);
        }else{
            m_free = m_head;
        }
        m_head = m_free;
        m_tail = m_free;
        m_free = removeHead(m_free);
    }

    template<typename T, typename ...ARGS>
    T* allocate(const ARGS& ... args){
        static_assert(sizeof(T) <= Block::SIZE, "Object should be 1024 bytes or less");
        Block* b = m_tail;



        int remainign = Block::SIZE - b->ptr;
        if(remainign < sizeof(T)){
            b = m_free;
            if(b == nullptr){
                b = new Block();
                addBlock(b);
            }else{
                m_free = removeHead(b);
            }
        }
        T* ptr = (T*)(b->buffer + b->ptr);
        b->ptr += sizeof(T);
        return new (ptr)T(args...);
    }

    ~DynamicStack(){
        Block* b = nullptr;
        if(m_head != nullptr){
            b = m_head;
            if(m_free != nullptr){
                insertNext(m_tail,m_free);
            }
        }
        else if(m_free != nullptr){
            b = m_free;
            if(m_head != nullptr){
                insertNext(m_free,m_head);
            }
        }else{
            assert(false);
        }
        
        do{
            Block* p = b;
            b = b->next;
            delete p;
        }while(b != nullptr);
    }
private:
    void insertNext(Block* b, Block* next){
        assert(b != nullptr);
        next->next = b->next;
        b->next    = next;
    }

    Block* removeHead(Block* b){
        Block* ret = b->next;
        b->next = nullptr;
        return ret;
    }

    void addBlock(Block* b){
        insertNext(m_tail,b);
        m_tail = b;
    }
private:
    Block* m_head = nullptr;
    Block* m_tail = nullptr;
    Block* m_free = nullptr;
};
#endif