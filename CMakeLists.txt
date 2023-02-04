cmake_minimum_required(VERSION 3.24)
project(webserv)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_COMPILER c++)

add_compile_options(-Wall -Wextra -Werror)

include_directories(inc)

add_executable(webserv
		src/main.cpp src/Server.cpp src/Client.cpp src/Conf.cpp src/Request.cpp src/Response.cpp)