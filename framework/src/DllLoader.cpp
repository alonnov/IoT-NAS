/********************************
 Name:     Alon Nov
 Exercise: DLL loader / Dir monitor
 Date:	   19.03.2025
 Reviewer: Gil Freedman

********************************/

#include <dlfcn.h>          // dlopen, dlclose
#include <stdexcept>        // std::runtime_error

#include "DllLoader.hpp"    // API, std::unordered_map, std::string

namespace ilrd
{
namespace details
{

DllLoader::~DllLoader()
{
    for(auto& iter : m_plugins)
    {
        dlclose(iter.second);
    }
}

void DllLoader::Load(const std::string& path_name_)
{   
    handle handle = dlopen(path_name_.c_str(), RTLD_LAZY);
    if(!handle)
    {
        throw std::runtime_error("dlopen error");
    }

    m_plugins[path_name_] = handle;
}


void DllLoader::Unload(const std::string& path_name_)
{
    const char* str = path_name_.c_str();

    dlclose(m_plugins.at(str));

    m_plugins.erase(str);
}


} //namespace details
} //namespace ilrd



