/* *****************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025   

********************************/

#include <iostream>

#include "FileManager.hpp" 
#include "Handleton.hpp"
#include "CommandReadResponse.hpp"
#include "CommandWriteResponse.hpp"
#include "FileManager.hpp"
#include "udp.h"
#include "Framework.hpp"
#include "Handleton.hpp"
#include "MasterProxy.hpp"
#include "ResponseManager.hpp"
#include "MinionManager.hpp"
#include "utils.hpp"  

namespace ilrd
{
namespace details   
{

std::shared_ptr<ICommand> CreateReadCommand()
{
    return std::make_shared<CommandReadResponse>(CommandReadResponse());
};

std::shared_ptr<ICommand> CreateWriteCommand()
{
    return std::make_shared<CommandWriteResponse>(CommandWriteResponse());
};


} //namespace details
} //namespace ilrd

using namespace:: ilrd::details;

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        return 1;
    }

    std::shared_ptr<IInputProxy> master = std::make_shared<MasterProxy>(argv[1], argv[2]);

    int fd = static_pointer_cast<MasterProxy>(master)->GetFD();

    std::list<Framework::FrameworkFDs> input_proxy = {{fd, READ, master}, {fd, WRITE, master}};
                                                        
    std::list<std::pair<int, std::function<std::shared_ptr<ICommand>()>>> creators = 
        {std::make_pair(0, CreateReadCommand), 
            std::make_pair(1, CreateWriteCommand)};

    ilrd::Handleton<FileManager>::GetInstance();   
    
    Framework fw(input_proxy, creators, "./plugins");
    
    fw.Run();

    return 0;
}


