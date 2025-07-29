/********************************
 Name:     Alon Nov
 Exercise: DLL loader / Dir monitor
 Date:	   19.03.2025
 Reviewer: Gil Freedman

********************************/

#ifndef ILRD_RD1645_DLLLOADER_HPP
#define ILRD_RD1645_DLLLOADER_HPP

#include <unordered_map>    //unordered_map
#include <string>           // std::string

typedef void* handle;

namespace ilrd
{
namespace details
{


class DllLoader
{
public:
    ~DllLoader();
    void Load(const std::string& path_name);
    void Unload(const std::string& path_name);

private:
    std::unordered_map<std::string, handle> m_plugins;
};


} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_DLLLOADER_HPP

