#pragma once

#include <atomic>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

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

    void OffServer();

private:
    ServerManager* server_;
    int server_fd;
    sockaddr_in address_;
    std::atomic<bool> IsListening = true;
    std::vector<std::thread> threads_;
    void CompleteCommand(int socket_id);

};