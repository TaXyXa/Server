#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <netinet/in.h>

#include "TspServer.h"
#include "ServerManager.h"

TCPServer::TCPServer(ServerManager& server, sockaddr_in new_address) 
: server_(&server), address_(new_address)
{
    Initialization();
    CheckConnections();
}

TCPServer::~TCPServer() {
    std::cout << "TCP: Bye" << std::endl;
}

bool TCPServer::Initialization() {
    int opt = 1;
    
    server_fd = socket(address_.sin_family, SOCK_STREAM, 0);
    if (server_fd == -1) {
        //error
        return false;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        //error 
        return false;
    }

    if (bind(server_fd, (sockaddr*)&address_, sizeof(address_)) < 0) {
        //error
        return false;
    }

    if (listen(server_fd, 3) < 0) {
        //error
        return false;
    }

    return true;
    //server started
    //call connections
}

void TCPServer::CheckConnections() {
    sockaddr_in address;
    int new_socket;
    int address_lenght = sizeof(address);

    int count = 0;
    while (IsListening) {
        count++;
        if (count > 5) {
            IsListening = false;
        }

        new_socket = accept(server_fd, (sockaddr*)&address, (socklen_t*)&address_lenght);
        if (new_socket < 0) {
            //error
            std::cout << "Continue" << std::endl;
            continue;
        }
        std::cout << "TCP: New Connection!" << std::endl;
        std::thread new_thread(&TCPServer::CompleteCommand, this, new_socket);
        new_thread.detach();
    }
    std::cout << "TCP: Server no listening!" << std::endl;
}

void TCPServer::CompleteCommand(int socket_id) {
    //read
    char buffer[1000] {0};
    int read_num = read(socket_id, buffer, 1000);
    if (read_num<0) {
        //error reading
        return;
    }
    std::string answer = server_->CompliteRequest(buffer);
    send(socket_id, answer.c_str(), answer.size(), 0); 
    close(socket_id);
    //IsListening = false;
}