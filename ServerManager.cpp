#include <iostream>
#include <netinet/in.h>

#include "ServerManager.h"
#include "TspServer.h"


ServerManager::ServerManager() 
{
    address_.sin_family = AF_INET;
    address_.sin_addr.s_addr = INADDR_ANY;
    address_.sin_port = htons(8001);
    request_manager_ = std::make_unique<RequestManager> (*this);
    tsp_server_ = std::make_unique<TCPServer> (*this, address_);
    std::cout << "Server Created" << std::endl;
}

std::string ServerManager::CompliteRequest(const char *request)
{
    return request_manager_->CompliteRequest(request);
}

std::string ServerManager::SetConfiguration(const std::string &ip, const std::string &mask)
{
    std::string delete_config = "ip addr flush dev enp0s3";
    std::string add_config = "ip addr add " + ip + "/" + mask + " dev enp0s3";
    std::string up_config = "ip link set enp0s3 up";
    if (system(delete_config.c_str()) == 0 && system(add_config.c_str()) == 0 &&
    system(up_config.c_str()) == 0 ) {
        std::cout << "Try create server" << std::endl;
        //tsp_server_ = std::make_unique<TCPServer> (*this, address_);
        return "Server reconfigurated /n";
    } else {
        return "Failed reconfiguration server /n";
    }
}