#include <string>
#include <nlohmann/json.hpp>
#include <unordered_map>

#include "RequestManager.h"
#include "ServerManager.h"

RequestManager::RequestManager(ServerManager& server)
:server_(&server)
{

}

std::string RequestManager::CompliteRequest(const char *request)
{
    nlohmann::json json_request = ParseRequest(request);
    std::unordered_map<std::string, const std::string> map_request;
    for (auto& [key, value] : json_request.items()) {
        map_request.emplace(key, value);
    }
    auto type_iter = map_request.find("type");
    if (type_iter == map_request.end()) 
    {
        return "Unknow request type.";
    }

    if (type_iter->second == "GetConfiguration") 
    {
        return GetConfiguration();
    } 
    else if (type_iter->second == "SetConfiguration") 
    {
        auto ip_iter = map_request.find("ip");
        auto mask_iter = map_request.find("mask");
        if (ip_iter == map_request.end() || mask_iter == map_request.end()) {
            return "Not find ip or mask.";
        }
        return server_->SetConfiguration(ip_iter->second, mask_iter->second);
    } 
    else if (type_iter->second == "Error") 
    {
        return "Parse request error.";
    }
    return "Unknow error.";
}


std::string RequestManager::GetConfiguration() const
{
    return "Get.";
}

nlohmann::json RequestManager::ParseRequest(const char *request)
{
    nlohmann::json request_json;
    if (nlohmann::json::accept(request)) {
        request_json = nlohmann::json::parse(request);
    } else {
        request_json["type"] = "Error";
    }
    return request_json;
}

