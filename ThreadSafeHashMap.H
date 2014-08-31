#ifndef UTIL_THREAD_SAFE_HASHMAP_H
#define UTIL_THREAD_SAFE_HASHMAP_H

#include "ThreadSafeStack.H"

#include <functional>
#include <pair>

namespace util
{
  template<class Key, class Value, 
           class Hash = std::hash<key>, 
           class Equal=std::equal_to<Key>>
  class ThreadSafeHashMap
  {
    public:
      typedef Key                     KeyType;
      typedef std::pair<Key, Value>   ValueType;
      typedef Value                   MappedType;
      typedef ValueType&              Reference;
      typedef ValueType*              Pointer;
      typedef const ValueType&        ConstReference;
      typedef const ValueType*        ConstPointer;
      
      typedef ThreadSafeHashMap<Key, Value, Hash, Equal> SelfType;

      struct Node: public NodeBase<Node>
      {
        Node(LinkType next, size_t vhash, const ValueType& pair): 
                NodeBase<Node>(next), hashValue(vhash), value(pair) 
        {
        }

        size_t hashValue;
        ValueType value;
      };
  };
}


#endif


