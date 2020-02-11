#ifndef _EVENT_QUE_H_
#define _EVENT_QUE_H_
#include <stdint.h>
#include <stddef.h>
#include <type_traits>

class EventQue{
private:
    static constexpr uint32_t BLOCK_SIZE = 2<<13;
    static constexpr uint32_t MAX_BLOCK  = 1024;
    static constexpr uint32_t N_BUFFER   = 2;
   
    template<typename T>
    struct EventInternal{
        size_t   size = sizeof(EventInternal<T>);
        uint32_t type;
        T        data;

        EventInternal(int t, const T& d){
            type = t;
            data = d;
        }
    };
    struct Block{
        uint32_t events = 0;
        uint32_t ptr    = 0;
        uint8_t  buffer[EventQue::BLOCK_SIZE];
    };

    struct Array{
        Block*   m_blocks[EventQue::MAX_BLOCK];
        uint32_t m_blockCount = 0;
        uint32_t m_eventCount = 0;
    };
public:
    struct Event{
        uint32_t id;
        void*    data;

        template<typename T>
        T* ptr(){
            return (T*)data;
        }

        template<typename T>
        T& value(){
            return *ptr<T>();
        }
    };

    class const_iterator{
    public:
        const_iterator(EventQue& eq){
            m_buffer     = eq.backBuffer();
            m_block      = m_buffer->m_blockCount;
            m_blockptr   = m_buffer->m_blocks[m_block]->ptr;
            m_blockCount = m_buffer->m_blocks[m_block]->events - 1;  
        }   

        const_iterator(EventQue& eq, int event){
            m_buffer     = eq.backBuffer();
            m_blockCount = 0;
            m_blockptr   = 0;
            m_block      = 0;
            for(int i = 0; i < event; i++){
                next();
            }
            fillReturn();
        }

        const_iterator(const const_iterator& other){
            m_buffer     = other.m_buffer; 
            m_block      = other.m_block;
            m_blockptr   = other.m_blockptr;
            m_blockCount = other.m_blockCount;
            fillReturn();
        }

        const_iterator& operator=(const const_iterator& other){
            m_buffer     = other.m_buffer; 
            m_block      = other.m_block;
            m_blockptr   = other.m_blockptr;
            m_blockCount = other.m_blockCount;
            return *this;
        }

        bool operator==(const const_iterator& other){
            int delta = 0;
            delta += (m_block      - other.m_block);
            delta += (m_blockCount - other.m_blockCount);
            bool result = !delta;
            return result;
        }

        bool operator!=(const const_iterator& other){
            return !(*this==other);
        }

        const Event* operator->() const {
            return &m_return;
        }

        const Event& operator*() const {
            return m_return;
        }

        const_iterator operator++(int){
            const_iterator ret(*this);
            ret.next();
            return ret;
        }

        const_iterator& operator++(){
            next();
            return *this;
        }
    private:
        void next(){
            ++m_blockCount;
            Block*   b      = m_buffer->m_blocks[m_block];
            if(m_blockCount >= b->events){
                ++m_block;
                m_blockptr   = 0;
                m_blockCount = 0;
                b            = m_buffer->m_blocks[m_block];
            }else{
                m_blockptr += *((size_t*)(b->buffer + m_blockptr));
            }
            fillReturn();
        }

        void fillReturn(){
            Block*   b      = m_buffer->m_blocks[m_block];
            uint8_t*            dataptr  = b->buffer + m_blockptr; 
            EventInternal<int>* eventptr = (EventInternal<int>*)dataptr;

            m_return.id   = eventptr->type;
            m_return.data = &eventptr->data;
        }

    private:
        uint32_t m_block;
        uint32_t m_blockptr;
        uint32_t m_blockCount;

        Array*   m_buffer;
        Event    m_return;
    };
    
    EventQue(){
        for(int i =0; i < N_BUFFER; i++){
            Array* arr = m_buffer[i];
            arr->m_blocks[0] = new Block(); 
        }
    }

    template<typename T>
    void post(uint32_t id, const T& data){
        static_assert(std::is_trivially_default_constructible<T>::value, "Data must be trivialy constructable");
        static_assert(sizeof(EventInternal<T>) < BLOCK_SIZE, " Event size too large");
        EventInternal<T> e(id,data);
        Array* array = frontBuffer();

        Block* block = array->m_blocks[array->m_blockCount];
        ++array->m_eventCount;
        
        if(block->ptr + e.size >= BLOCK_SIZE){
            ++array->m_blockCount;
            if(array->m_blocks[array->m_blockCount] == nullptr){
                array->m_blocks[array->m_blockCount] = new Block();
            }
            block = array->m_blocks[array->m_blockCount];
            block->events = 0;
            block->ptr    = 0;
        }
        
        *((EventInternal<T>*)(block->buffer + block->ptr))=e; 
        block->ptr += e.size;
        ++block->events;
    }

    void swap(){
        m_bufferPtr = (m_bufferPtr + 1) % N_BUFFER;
        m_buffer[m_bufferPtr]->m_blockCount = 0;
        m_buffer[m_bufferPtr]->m_eventCount = 0;
    }

    auto begin(){
        return const_iterator(*this,0);
    }

    auto end(){
        return const_iterator(*this);
    }

    ~EventQue(){
        for(int i = 0; i < N_BUFFER; i++){
            delete m_buffer[i];
        }
    }
private: 
    Array* frontBuffer() const{
        return m_buffer[m_bufferPtr%N_BUFFER];
    }

    Array* backBuffer() const{
        return m_buffer[(m_bufferPtr-1)%N_BUFFER];
    }

    Array* const m_array_1          =  new Array();
    Array* const m_array_2          =  new Array();
    Array*       m_buffer[N_BUFFER] = {m_array_1,m_array_2}; 
    uint8_t      m_bufferPtr        = 0;
};


#endif