/********************************
 Name:     Alon Nov
 Exercise: factory
 Date:	   12.03.2025
 Reviewer: Gil Freedman

********************************/

#ifndef ILRD_RD1645_FACTORY_HPP
#define ILRD_RD1645_FACTORY_HPP

#include <unordered_map>
#include <functional>   // std::function
#include <memory>       // std::shared_ptr

namespace ilrd
{
namespace details
{
template <typename KEY, typename BASE, typename... ARGS>
class Factory
{
public:
    Factory();
    void Register(KEY, std::function<std::shared_ptr<BASE>(ARGS...)>);
    void Unregister(KEY);
    std::shared_ptr<BASE> Create(KEY, ARGS...);

    Factory(const Factory&) = delete;
    Factory operator=(const Factory&) = delete;
private:
    std::unordered_map<KEY, std::function<std::shared_ptr<BASE>(ARGS...)>> m_container;
};

template <typename KEY, typename BASE, typename... ARGS>
Factory<KEY, BASE, ARGS...>::Factory()
{
}

template <typename KEY, typename BASE, typename... ARGS>
void Factory<KEY, BASE, ARGS...>::Register(KEY key, std::function<std::shared_ptr<BASE>(ARGS...)> func)
{
    m_container[key] = func;
}


template <typename KEY, typename BASE, typename... ARGS>
void Factory<KEY, BASE, ARGS...>::Unregister(KEY key)
{
    m_container.erase(key);
}


template <typename KEY, typename BASE, typename... ARGS>
std::shared_ptr<BASE> Factory<KEY, BASE, ARGS...>::Create(KEY key, ARGS... args)
{
    return m_container.at(key)(args...);
}

} //namespace details
} //namespace ilrd



#endif //ILRD_RD1645_FACTORY_HPP



