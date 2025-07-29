/* *****************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025   

********************************/

#include "MinionProxy.hpp"
#include "Handleton.hpp"
#include "MinionManager.hpp"
#include "ResponseManager.hpp"
#include "CommandReadRequest.hpp"
#include "CommandWriteRequest.hpp"
#include "Framework.hpp"
#include "NBDProxy.hpp"
#include "NBD.hpp"
#include "ATaskArgs.hpp"

using namespace:: ilrd::details;

std::shared_ptr<ICommand> CreateReadCommand()
{
    return std::make_shared<CommandReadRequest>(CommandReadRequest());
};

std::shared_ptr<ICommand> CreateWriteCommand()
{
    return std::make_shared<CommandWriteRequest>(CommandWriteRequest());
};



int main(int argc, char** argv)
{
    if(argc < 4 || 1 == (argc % 2))
    {
        return 1;
    }

    NBD nbd;
    int fd = nbd.Init(argv[1], 1024 * 1024 * 8, (argc - 2) / 2);
    std::vector<std::shared_ptr<IMinionProxy>> minions;

    for(int i = 2; i < argc; i += 2)
    {
        minions.emplace_back(std::make_shared<MinionProxy>(argv[i], argv[i + 1]));
    }

    std::shared_ptr<NBDProxy> nbd_proxy = std::make_shared<NBDProxy>(fd);

    std::list<Framework::FrameworkFDs> input_proxies = {{fd, READ, nbd_proxy}, {fd, WRITE, nbd_proxy}};

    std::list<std::pair<int, std::function<std::shared_ptr<ICommand>()>>> creators = 
        {std::make_pair(0, CreateReadCommand), 
            std::make_pair(1, CreateWriteCommand)};

    ilrd::Handleton<MinionManager>::GetInstance()->Init(minions);
    ilrd::Handleton<ResponseManager>::GetInstance()->Init(nbd_proxy);

    Framework fw(input_proxies, creators, "./framework/plugins");

    fw.Run();
	
	return 0;
}