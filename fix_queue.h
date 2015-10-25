#ifndef UTIL_FIX_QUEUE_H
#define UTIL_FIX_QUEUE_H

#include <stdlib.h>
#include <stdint.h>
#include <utility>

namespace util
{
	template<typename T, uint32_t size>
	class fix_queue
	{
	public:

        static constexpr uint32_t element_size = sizeof(T);

		fix_queue() : buffer_(static_cast<char*>(malloc(size * element_size)))
		{
		}

		~fix_queue()
        {
            free(buffer_);
        }
        
        template<typename... U>
		bool enqueue(U&&... u)
		{
			if((head_ - tail_) == size)
			{
				return false;
			}
			
			new (buffer_ + (head_ % size) * element_size) T(std::forward<U>(u)...);
			++head_;
			return true;
		}

		T* peek()
		{
			if(head_ == tail_)
			{
				return nullptr;
			}

			return static_cast<T*>(static_cast<void*>(buffer_ + (tail_ % size) * element_size));
		}

		bool dequeue()
		{
			if(head_ == tail_)
			{
				return false;
			}
			
			++tail_;
			return true;
		}
	
	private:
		char* const buffer_ = nullptr;
		volatile uint64_t head_ = 0;
		volatile uint64_t tail_ = 0;
	};

} //namespace util

#endif
