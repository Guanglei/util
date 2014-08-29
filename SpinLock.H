#ifndef UTIL_SPINLOCK_H
#define UTIL_SPINLOCK_H

#include <pthread.h>

#include "LockBase.H"

namespace util
{
  class SpinLock: public LockBase<SpinLock>
  {
    public:
      SpinLock()
      {
        pthread_spin_init(&lock, PTHREAD_PROCESS_PRIVATE);
      }

      ~SpinLock()
      {
        pthread_spin_destroy(&lock);
      }
      
      void acquire()
      {
        pthread_spin_lock(&lock);
      }

      void release()
      {
        pthread_spin_unlock(&lock);
      }

    private:
      pthread_spinlock_t lock;
  };
}

#endif
