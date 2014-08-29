#ifndef UTIL_LOCKBASE_H
#define UTIL_LOCKBASE_H

namespace util
{
  template<class Lock>
  struct LockBase
  {
    struct Guard
    {
      Guard(Lock& l): lock(l)
      {
        lock.acquire();
      }

      ~Guard()
      {
        lock.release();
      }

      Lock& lock;
    };
    
    virtual void acquire() =0;
    virtual void release() =0;
  };
}

#endif
