/********************************
 Name:     Alon Nov
 Exercise: logging
 Date:	   17.03.2025
 Reviewer: Itai Hirschhorn

********************************/

#include <iomanip>
#include <memory>

#include "Logger.hpp"   // API


namespace ilrd
{
namespace details
{

///////////////////////////Logger///////////////////////////////
Logger::Logger(const std::string& file_path) :
m_log(file_path, std::ofstream::app),  
M_SEVERITY_LUT("DEBUG", "WARNING", "ERROR", "END")
{
    m_thread = std::jthread([this]() {ThreadFunc();});
}


Logger::~Logger()
{
    while(!m_log_queue.IsEmpty());

    m_log_queue.Push({"SESSION ENDED", static_cast<SEVERITY>(KILL)});
} 


void Logger::ThreadFunc()
{
    Logger::LogData log("", DEBUG);

    SEVERITY kill = static_cast<SEVERITY>(KILL);

    while(log.m_severity != kill)
    {
        m_log_queue.Pop(log);        
        m_log << std::put_time(std::localtime(&log.m_time), "%F %T\t\t") << 
        M_SEVERITY_LUT[log.m_severity] << "\t\t" << log.m_msg << std::endl;
    }

    m_log.close();
}


void Logger::Log(const std::string& str_, SEVERITY severity_)
{
#ifndef NDEBUG
    m_log_queue.Push({str_, severity_});
#else
    if (severity_ != DEBUG) 
    {
        m_log_queue.Push({str_, severity_});
    }
#endif
}

///////////////////////////LogData///////////////////////////////
Logger::LogData::LogData(const std::string& str_, SEVERITY severity_) : 
m_msg(str_), m_severity(severity_), m_time(time(NULL))
{
}


} //namespace details
} //namespace ilrd




