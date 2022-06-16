#ifndef __YNWAD_MUTEX_H__
#define __YNWAD_MUTEX_H__

#include <pthread.h>
#include "noncopyable.h"

namespace ynwad {

//局部锁的模板实现
template<class T>
class ScopedLockImpl {
public:
    ScopedLockImpl(T& mutex)
        :m_mutex(mutex) {
        m_mutex.lock();
        m_locked = true;
    }

    ~ScopedLockImpl() {
        unlock();
    }

    void lock() {
        if(!m_locked) {
            m_mutex.lock();
            m_locked = true;
        }
    }

    void unlock() {
        if(m_locked) {
            m_mutex.unlock();
            m_locked = false;
        }
    }

private:
    /// mutex
    T& m_mutex;
    bool m_locked;
};

class Spinlock : public Noncopyable {
public:
/// 局部锁
    typedef ScopedLockImpl<Spinlock> Lock;

    Spinlock() {
        pthread_spin_init(&m_mutex, 0);
    }

    ~Spinlock() {
        pthread_spin_unlock(&m_mutex);
    }

    void lock() {
        pthread_spin_lock(&m_mutex);
    }

    void unlock() {
        pthread_spin_unlock(&m_mutex);
    }

private:
    pthread_spinlock_t m_mutex;
};

}



#endif