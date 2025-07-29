/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#include "Logger.hpp"
#include "Handleton.hpp"
#include "MasterProxy.hpp"
#include "FileManager.hpp"    // API

namespace ilrd
{
namespace details   
{

FileManager::FileManager(const std::string& file_path_) : m_file(file_path_)
{
}

STATUS FileManager::Read(size_t offset_, size_t length_, std::shared_ptr<char[]> read_to_)
{
    try
    {
        Handleton<Logger>::GetInstance()->Log("FileManager: Read Called", Logger::DEBUG);
        
        std::unique_lock<std::mutex> lock(m_mtx);
            m_file.seekg(offset_);
            m_file.read(read_to_.get(), length_);
    }
    catch(...)
    {
        Handleton<Logger>::GetInstance()->Log("FileManager: Read Failed", Logger::ERROR);
        return FAILURE;
    }
    
    return SUCCESS;
}

STATUS FileManager::Write(size_t offset_, size_t length_, std::shared_ptr<char[]> write_from_)
{
    try
    {
        Handleton<Logger>::GetInstance()->Log("FileManager: Write Called", Logger::DEBUG);

        std::unique_lock<std::mutex> lock(m_mtx);
            m_file.seekp(offset_);
            m_file.write(write_from_.get(), length_);
            m_file.flush();
    }
    catch(...)
    {
        Handleton<Logger>::GetInstance()->Log("FileManager: Write Failed", Logger::ERROR);
        return FAILURE;
    }
    
    return SUCCESS;
}


} //namespace details
} //namespace ilrd




