/********************************
 Name:     Alon Nov
 Exercise: logging
 Date:	   17.03.2025
 Reviewer: Itai Hirschhorn

********************************/

#ifndef ILRD_RD1645_LOGGER_HPP
#define ILRD_RD1645_LOGGER_HPP

#include <thread>       //jthread
#include <fstream>      //std::ofstream

#include "WaitQ.hpp"    // WaitQ
#include "Handleton.hpp"// Handleton

// Debug logging control - uses standard NDEBUG macro
// No additional code needed - existing Logger::DEBUG calls will be handled automatically

namespace ilrd
{
namespace details
{

class Logger
{
public:
    enum SEVERITY
    {
        DEBUG = 0,
        WARNING = 1,
        ERROR = 2
    };
    
    ~Logger();
    void Log(const std::string& entry_message, SEVERITY entry_type);
    
    private:
    
    struct LogData
    {
        LogData(const std::string&, SEVERITY);
        std::string m_msg;
        SEVERITY m_severity;
        std::time_t m_time;
    }; 
    
    Logger(const std::string& path_name = "./log.txt");
    void ThreadFunc();
    std::ofstream m_log;
    std::jthread m_thread;
    WaitQ<LogData> m_log_queue;
    const std::string M_SEVERITY_LUT[4];
    friend ilrd::Handleton<Logger>;

    enum SYSTEM
    {
        KILL = 3
    };
};


} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_LOGGER_HPP



