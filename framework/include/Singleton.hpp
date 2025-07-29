/********************************
 Name:     Alon Nov
 Exercise: singleton
 Date:	   16.03.2025
 Reviewer: Ofir Wijsboom

********************************/

#ifndef ILRD_RD1645_SINGLETON_HPP
#define ILRD_RD1645_SINGLETON_HPP

#include <mutex>    // std::mutex, unique_lock
#include <atomic>   // std::atomic
#include <memory>   //std::shared_ptr, std::make_unique


namespace ilrd
{

template <typename T>
class Singleton
{
public:
    // gets reference to the created singleton object
    static T* GetInstance();
    ~Singleton() = delete;
    Singleton() = delete;
    Singleton(const Singleton<T>&) = delete;
    Singleton operator=(const Singleton<T>&) = delete;

private:
    static void Destroy();
    static std::mutex m_mtx;
    static std::atomic<T*> m_instance;
};

template <typename T>
std::mutex Singleton<T>::m_mtx;

template <typename T>
std::atomic<T*> Singleton<T>::m_instance{nullptr};

template <typename T>
T* Singleton<T>::GetInstance()
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
void Singleton<T>::Destroy()
{
    delete m_instance.load(std::memory_order_relaxed);

    m_instance.store(reinterpret_cast<T*>(0xDEADBEEF));
}



} //namespace ilrd



#endif //ILRD_RD1645_SINGLETON_HPP



