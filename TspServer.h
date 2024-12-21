#pragma once

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <atomic>

#include "ServerManager.h"

class ServerManager;

class TCPServer {
public:
//need add path to log output
    TCPServer(ServerManager& server, sockaddr_in new_address);

    ~TCPServer();

    //return false if error 
    bool Initialization();

    void CheckConnections();


private:
    ServerManager* server_;
    int server_fd;
    sockaddr_in address_;
    std::atomic<bool> IsListening = true;

    void CompleteCommand(int socket_id);

};