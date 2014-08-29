#ifndef UTIL_THREAD_SAFE_STACK_H
#define UTIL_THREAD_SAFE_STACK_H


#include "SpinLock.H"

#include <type_traits>

namespace util 
{
  //Node must inherit NodeBase
  template<class Node>
  struct NodeBase
  {
    typedef Node* volatile LinkType;

    NodeBase(LinkType next = NULL):pNext(next) {}
    LinkType pNext;
  };
  
  template<class Node>
  class ThreadSafeStack
  {
    public:
      typedef typename NodeBase<Node>::LinkType LinkType;
      
      static_assert(std::is_base_of<NodeBase<Node>, Node>::value,
        "ThreadSafeStack only take node type inherit NodeBase");
      
    public:
      ThreadSafeStack() : pHead(NULL) {}

      LinkType peek() { return pHead; }

      void push(LinkType node)
      {
        SpinLock::Guard g(lock);

        node->pNext = pHead;
        pHead = node;
      }

      LinkType pop()
      {
        SpinLock::Guard g(lock);

        Node* ret = pHead;
        if(ret)
        {
          pHead = ret->pNext;
        }
        return ret;
      }

      bool empty() const { return pHead == NULL; }
    
    private:

      LinkType pHead;
      SpinLock lock;
  };
}

#endif
