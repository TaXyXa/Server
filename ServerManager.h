#pragma once

#include <netinet/in.h>
#include <memory>

#include "TspServer.h"
#include "RequestManager.h"

class TCPServer;
class RequestManager;

class ServerManager {
public:
    ServerManager();

    std::string CompliteRequest(const char *request);

    std::string SetConfiguration(const std::string &ip, const std::string &mask);

private:
    sockaddr_in address_;
    std::unique_ptr<RequestManager> request_manager_;
    std::unique_ptr<TCPServer> tsp_server_;

};