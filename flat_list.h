#ifndef UTIL_FLAT_LIST_H
#define UTIL_FLAT_LIST_H

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <utility>
#include <new>

namespace util
{
    template<typename T>
    class flat_list
    {
        struct bind
        {
            template<typename... arg_list_t>
            bind(arg_list_t&&... args) : 
                value(std::forward<arg_list_t>(args)...),
                next(nullptr)
            {
            }
            
            T value;
            bind* next;
        };
    
        static constexpr uint32_t bind_size = sizeof(bind);

        struct iterator_impl_t
        {
            iterator_impl_t(bind* u = nullptr) : underlying(u) {}         
            
            T& operator *()
            {
                return underlying->value;
            }
            
            iterator_impl_t& operator++ ()
            {
                if(underlying)
                {
                    underlying = underlying->next;
                }

                return *this;
            }

            iterator_impl_t operator++(int)
            {
                iterator_impl_t ret(underlying);
                if(underlying)
                {
                    underlying = underlying->next;    
                }

                return ret;
            }
            
            bool operator ==(const iterator_impl_t& rhs)
            {
                return underlying == rhs.underlying;
            }

            bind* underlying;
        };

        static iterator_impl_t invalid_iter;

    public:

        using iterator_t = iterator_impl_t;
        using const_iterator_t = const iterator_impl_t;

        flat_list(uint32_t count) : 
            total_count(count), used_count(0)
        {
            assert(count != 0);

            buffer = static_cast<char*>(calloc(count, sizeof(bind)));
        }

        ~flat_list()
        {
            for(uint32_t i = 0; i < used_count; ++i)
            {
                bind* temp = reinterpret_cast<bind*>(buffer + i*bind_size);
                temp->value.~T();
                temp->next = nullptr;
            }

            total_count = 0;
            used_count = 0;
            free(buffer);
        }
        
        template<typename... arg_list_t>
        iterator_t emplace_back(arg_list_t&&... args)
        {
            if(total_count == used_count)
            {
                return invalid_iter;
            }
            
            char* next_block = (buffer + used_count * bind_size);
            bind* next_item = reinterpret_cast<bind*>(next_block);
            new (&(next_item->value)) 
                T (std::forward<arg_list_t>(args)...);
            
            ++used_count;

            if(used_count > 1)
            {
                bind* last_item = reinterpret_cast<bind*>(next_block - bind_size);
                last_item->next = next_item;
            }

            bind* first_item = reinterpret_cast<bind*>(buffer);
            next_item->next = first_item;
            
            return iterator_t(next_item);
        }

        iterator_t begin()
        {
            if(used_count == 0)
            {
                return invalid_iter;
            }

            bind* first_item = reinterpret_cast<bind*>(buffer);
            return iterator_t(first_item);
        }

        iterator_t end()
        {
            return invalid_iter;
        }

        uint32_t total_count;
        uint32_t used_count;

        char* buffer;
    };

    template<typename T>
    typename flat_list<T>::iterator_impl_t flat_list<T>::invalid_iter;
}

#endif
