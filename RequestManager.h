#pragma once

#include <nlohmann/json.hpp>
#include <string>

#include "ServerManager.h"

class ServerManager;

class RequestManager {
public:

    RequestManager(ServerManager& server);

    std::string CompliteRequest(const char* request);

private:
    ServerManager* server_;

    std::string SetConfiguration(const std::string& ip, const std::string& mask);

    std::string GetConfiguration() const;

    nlohmann::json ParseRequest(const char* request);

};

