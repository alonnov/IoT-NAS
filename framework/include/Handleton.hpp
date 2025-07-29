/********************************
 Name:     Alon Nov
 Exercise: handleton
 Date:	   16.03.2025
 Reviewer: Shani Davidian

********************************/

#ifndef ILRD_RD1645_HANDLETON_HPP
#define ILRD_RD1645_HANDLETON_HPP

#include <mutex>            // std::mutex, unique_lock
#include <atomic>           // std::atomic

namespace ilrd
{
    
#ifdef HANDLETON_IMP

template <typename T>
class Handleton
{
public:
    // gets reference to the created Handleton object
    static T* GetInstance();
    ~Handleton() = delete;
    Handleton() = delete;
    Handleton(const Handleton<T>&) = delete;
    Handleton operator=(const Handleton<T>&) = delete;

private:
    static void Destroy();
    static std::mutex m_mtx;
    static std::atomic<T*> m_instance;
};

template <typename T>
std::mutex Handleton<T>::m_mtx;

template <typename T>
std::atomic<T*> Handleton<T>::m_instance{nullptr};

template <typename T>
T* Handleton<T>::GetInstance()
{
    if(m_instance.load(std::memory_order_relaxed) == reinterpret_cast<T*>(0xDEADBEEF))
    {
        throw std::logic_error("use-after-free");
    }
    
    T* res = m_instance.load(std::memory_order_acquire);

    if(res == nullptr)
    {
        std::unique_lock lock(m_mtx);
        res = m_instance.load(std::memory_order_relaxed);

        if(res == nullptr)
        {
            m_instance.store(new T, std::memory_order_release);
            res = m_instance.load(std::memory_order_relaxed);

            std::atexit(Destroy);
        }
    }

    return res;
}

template <typename T>
void Handleton<T>::Destroy()
{
    delete m_instance.load(std::memory_order_relaxed);

    m_instance.store(reinterpret_cast<T*>(0xDEADBEEF));
}


#else

template <typename T>
class Handleton
{
public:
    // gets refrence to the created Handleton object
    static T* GetInstance();

    Handleton() = delete;
    ~Handleton() = delete;
    Handleton(const Handleton<T>&) = delete;
    Handleton operator=(const Handleton<T>&) = delete;
};


#endif // HANDLETON_IMP

} //namespace ilrd

#endif //ILRD_RD1645_HANDLETON_HPP



