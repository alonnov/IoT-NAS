/********************************
 Name:     Alon Nov
 Exercise: thread pool
 Date:	   10.03.2025   
 Reviewer: Itai Hirschhorn

********************************/

#include "FunctionTask.hpp" // API

using namespace ilrd::details;

///////////////////////FunctionTask//////////////////////////

FunctionTask::FunctionTask(std::function<void()> task) : m_func(std::bind(task))
{
}

FunctionTask::~FunctionTask()
{
}

void FunctionTask::Run()
{
    m_func();
}

