#include <atomic>
#include <cassert>
#include <ostream>

template<uint32_t ELE_SIZE, uint32_t ELE_NUM, uint32_t MAX_ACQUIRE_NUM>
struct spsc_queue
{
    static_assert(ELE_SIZE > 0, "ELE_SIZE should be greater than 0");
    static_assert(ELE_NUM > 0, "ELE_NUM should be greater than 0");
    static_assert(ELE_NUM >= MAX_ACQUIRE_NUM, "ELE_NUM should be greater than MAX_ACQUIRE_NUM");

    using element_t = char[ELE_SIZE];
    element_t data[ELE_NUM + MAX_ACQUIRE_NUM] = {};  
    
    uint64_t pending_p_cur = 0;
    std::atomic<uint64_t> p_cur{0};

    uint64_t pending_c_cur = 0;
    std::atomic<uint64_t> c_cur{0};


    public:

        char* acquire_for_write(uint32_t num)
        {
            assert(num > 0 && num <= MAX_ACQUIRE_NUM);

            if(num == 0 || num > MAX_ACQUIRE_NUM || pending_p_cur)
            {
                return nullptr;
            }

            auto current_p_cur = p_cur.load(std::memory_order::memory_order_relaxed);
            
            auto mod = current_p_cur % ELE_NUM;

            int  acquire_start_offset = 0;

            if(mod < (ELE_NUM - 1) && (mod + num) >= ELE_NUM)
            {
                pending_p_cur = current_p_cur + (ELE_NUM - 1 - mod);
                acquire_start_offset = mod + 1;
            }
            else
            {
                pending_p_cur = current_p_cur + num;
                acquire_start_offset = (current_p_cur + 1) % ELE_NUM;
            }

            auto current_c_cur = c_cur.load(std::memory_order::memory_order_acquire);
            if((pending_p_cur - current_c_cur) > ELE_NUM)
            {
                pending_p_cur = 0;
                return nullptr;
            }
            
            return reinterpret_cast<char*>(&data[acquire_start_offset]);
        }

        void finish_write()
        {
            if(!pending_p_cur)
            {
                return;
            }

            p_cur.store(pending_p_cur, std::memory_order::memory_order_release);
            pending_p_cur = 0;
        }

        const char* acquire_for_read()
        {
            if(pending_c_cur)
            {
                return nullptr;
            }

            auto current_c_cur = c_cur.load(std::memory_order::memory_order_consume);
            auto current_p_cur = p_cur.load(std::memory_order::memory_order_relaxed);

            if(current_c_cur >= current_p_cur)
            {
                return nullptr;
            }

            pending_c_cur = current_c_cur + 1;

            return reinterpret_cast<const char*>(&data[pending_c_cur % ELE_NUM]);

        }

        void finish_read(uint32_t num)
        {
            assert(num > 0 && num <= MAX_ACQUIRE_NUM);

            if(!pending_c_cur || num == 0 || num > MAX_ACQUIRE_NUM)
            {
                return;
            }
            
            auto current_c_cur = c_cur.load(std::memory_order::memory_order_relaxed);

            auto mod = current_c_cur % ELE_NUM;
            
            if(mod < (ELE_NUM - 1) && (mod + num) >= ELE_NUM)
            {
                c_cur.store(current_c_cur + (ELE_NUM - 1 - mod), std::memory_order::memory_order_release);
            }
            else
            {
                c_cur.store(current_c_cur + num, std::memory_order::memory_order_release);
            }

            pending_c_cur = 0;
        }

        void print(std::ostream& os)
        {
            os << "current write cur: [" << p_cur.load(std::memory_order::memory_order_acquire) 
                << "], current read cur: [" << c_cur.load(std::memory_order::memory_order_acquire) << "]" << std::endl;
        }
};


