#ifndef __YNWAD_MUTEX_H__
#define __YNWAD_MUTEX_H__

// #include <pthread.h>
#include <mutex>
#include <shared_mutex>
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

    Spinlock()
        :m_mutex(ATOMIC_FLAG_INIT){

    }

    ~Spinlock() {
        unlock();
    }

    void lock() {
        // pthread_spin_lock(&m_mutex);
        m_mutex.test_and_set(std::memory_order_acquire);
    }

    void unlock() {
        // pthread_spin_unlock(&m_mutex);
        // m_mutex.unlock();
        m_mutex.clear(std::memory_order_release);
    }

private:
    // pthread_spinlock_t m_mutex;
    std::atomic_flag m_mutex;
};

/**
 * @brief 互斥量
 */
class Mutex : Noncopyable {
public: 
    /// 局部锁
    typedef ScopedLockImpl<Mutex> Lock;

    /**
     * @brief 构造函数
     */
    Mutex() {
        // pthread_mutex_init(&m_mutex, nullptr);
    }

    /**
     * @brief 析构函数
     */
    ~Mutex() {
        // pthread_mutex_destroy(&m_mutex);
    }

    /**
     * @brief 加锁
     */
    void lock() {
        // pthread_mutex_lock(&m_mutex);
        m_mutex.lock();
    }

    /**
     * @brief 解锁
     */
    void unlock() {
        // pthread_mutex_unlock(&m_mutex);
        m_mutex.unlock();
    }
private:
    /// mutex
    // pthread_mutex_t m_mutex;
    std::mutex  m_mutex;
};

/**
 * @brief 局部读锁模板实现
 */
template<class T>
struct ReadScopedLockImpl {
public:
    /**
     * @brief 构造函数
     * @param[in] mutex 读写锁
     */
    ReadScopedLockImpl(T& mutex)
        :m_mutex(mutex) {
        m_mutex.rdlock();
        m_locked = true;
    }

    /**
     * @brief 析构函数,自动释放锁
     */
    ~ReadScopedLockImpl() {
        unlock();
    }

    /**
     * @brief 上读锁
     */
    void lock() {
        if(!m_locked) {
            m_mutex.rdlock();
            m_locked = true;
        }
    }

    /**
     * @brief 释放锁
     */
    void unlock() {
        if(m_locked) {
            m_mutex.unrdlock();
            m_locked = false;
        }
    }
private:
    /// mutex
    T& m_mutex;
    /// 是否已上锁
    bool m_locked;
};

/**
 * @brief 局部写锁模板实现
 */
template<class T>
struct WriteScopedLockImpl {
public:
    /**
     * @brief 构造函数
     * @param[in] mutex 读写锁
     */
    WriteScopedLockImpl(T& mutex)
        :m_mutex(mutex) {
        m_mutex.wrlock();
        m_locked = true;
    }

    /**
     * @brief 析构函数
     */
    ~WriteScopedLockImpl() {
        unlock();
    }

    /**
     * @brief 上写锁
     */
    void lock() {
        if(!m_locked) {
            m_mutex.wrlock();
            m_locked = true;
        }
    }

    /**
     * @brief 解锁
     */
    void unlock() {
        if(m_locked) {
            m_mutex.unwrlock();
            m_locked = false;
        }
    }
private:
    /// Mutex
    T& m_mutex;
    /// 是否已上锁
    bool m_locked;
};

/**
 * @brief 读写互斥量
 */
class RWMutex : Noncopyable{
public:

    /// 局部读锁
    typedef ReadScopedLockImpl<RWMutex> ReadLock;

    /// 局部写锁
    typedef WriteScopedLockImpl<RWMutex> WriteLock;

    /**
     * @brief 构造函数
     */
    RWMutex() {
        // pthread_rwlock_init(&m_lock, nullptr);
    }
    
    /**
     * @brief 析构函数
     */
    ~RWMutex() {
        unwrlock();
    }

    /**
     * @brief 上读锁
     */
    void rdlock() {
        // pthread_rwlock_rdlock(&m_lock);
        m_lock.lock_shared();
    }

    /**
     * @brief 上写锁
     */
    void wrlock() {
        // pthread_rwlock_wrlock(&m_lock);
        // std::unique_lock<std::shared_mutex> lock(m_lock);
        m_lock.lock();
    }

    /**
     * @brief 解读锁
     */
    void unrdlock() {
        // pthread_rwlock_unlock(&m_lock);
        m_lock.unlock_shared();
    }

    /**
     * @brief 解写锁
     */
    void unwrlock() {
        // pthread_rwlock_unlock(&m_lock);
        m_lock.unlock();
    }
private:
    /// 读写锁
    // pthread_rwlock_t m_lock;
    std::shared_mutex m_lock;
};

}



#endif