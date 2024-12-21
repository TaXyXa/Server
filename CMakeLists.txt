cmake_minimum_required(VERSION 3.10)

project(Server)

set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_STANDART_REQUIRED True)

set(CMAKE_CXX_COMPILER g++)

add_executable(Server main.cpp TspServer.cpp ServerManager.cpp RequestManager.cpp)

