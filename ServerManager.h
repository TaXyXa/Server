#pragma once

#include <netinet/in.h>
#include <memory>
#include <thread>
#include <functional>

#include "TspServer.h"
#include "RequestManager.h"

class TCPServer;
class RequestManager;

class ServerManager {
public:
    ServerManager();

    void WaitRestart();

    std::string CompliteRequest(const char *request);

    std::string SetConfiguration(const std::string &ip, const std::string &mask);

    std::string GetConfiguration() const;

    void RestartServer();

private:
    sockaddr_in address_;
    std::shared_ptr<RequestManager> request_manager_;
    std::shared_ptr<TCPServer> tsp_server_;
    std::thread tsp_thread_;
    bool need_restart = false;
    void RestartTSP();
};