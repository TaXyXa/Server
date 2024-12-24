#include <iostream>
#include <netinet/in.h>
#include <thread>
#include <chrono>

#include "ServerManager.h"
#include "TspServer.h"


ServerManager::ServerManager() 
{
    address_.sin_family = AF_INET;
    address_.sin_addr.s_addr = INADDR_ANY;
    address_.sin_port = htons(8001);
    request_manager_ = std::make_unique<RequestManager> (*this);
    tsp_server_ = std::make_shared<TCPServer> (*this, address_);
    tsp_thread_ = std::thread([tsp_server_ = this->tsp_server_](){
        tsp_server_->Initialization();
        tsp_server_->CheckConnections();
    });
    std::cout << "Server Created" << std::endl;
    WaitRestart();
}

void ServerManager::WaitRestart()
{
    while(true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (need_restart) {
            tsp_thread_.join();
            RestartTSP();
        }
    }

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
        //std::thread restart(&ServerManager::RestartTSP, this);
        //restart.detach();
        return "Server reconfigurated.";
    } else {
        return "Failed reconfiguration server.";
    }
}

std::string ServerManager::GetConfiguration() const
{
    return "Get.";
}

void ServerManager::RestartServer()
{
    tsp_server_->OffServer();
    need_restart = true;
}

//new thread can created between waiting and making!
void ServerManager::RestartTSP()
{
    std::cout << "Make tcp" << std::endl;
    tsp_server_ = std::make_unique<TCPServer> (*this, address_);
        tsp_thread_ = std::thread([tsp_server_ = this->tsp_server_](){
        tsp_server_->Initialization();
        tsp_server_->CheckConnections();
    });
    std::cout << "Server Created" << std::endl;
    need_restart = false;
}
