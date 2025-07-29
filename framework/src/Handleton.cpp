/********************************
 Name:     Alon Nov
 Exercise: handleton
 Date:	   16.03.2025
 Reviewer: Shani Davidian

********************************/

#define HANDLETON_IMP

#include "ATaskArgs.hpp"
#include "FileManager.hpp"
#include "Message.hpp"
#include "Logger.hpp"       // Logger
#include "Scheduler.hpp"    // Scheduler
#include "ThreadPool.hpp"   // ThreadPool
#include "Factory.hpp"      // Factory
#include "Framework.hpp"    // Framework
#include "ResponseManager.hpp"
#include "MinionManager.hpp"
#include "Handleton.hpp"    //API

template class ilrd::Handleton<ilrd::details::Logger>;

template class ilrd::Handleton<ilrd::details::Scheduler>;

template class ilrd::Handleton<ilrd::details::ThreadPool>;

template class ilrd::Handleton<ilrd::details::Factory<int, ilrd::details::ICommand>>;

template class ilrd::Handleton<ilrd::details::Factory<uint32_t, ilrd::details::ATaskArgs, std::shared_ptr<ilrd::details::ANBDArgs>>>;

template class ilrd::Handleton<ilrd::details::ResponseManager>;

template class ilrd::Handleton<ilrd::details::MinionManager>;

template class ilrd::Handleton<ilrd::details::Reactor>;

template class ilrd::Handleton<ilrd::details::FileManager>;

