cmake_minimum_required(VERSION 3.25)
project(connector LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
#find_package(libpqxx REQUIRED)
#find_package(OpenSSL REQUIRED)

add_executable(connect database_connector.cpp main.cpp)

target_link_options(connect PRIVATE -L/opt/homebrew/Cellar/libpqxx/7.7.5/lib -lpqxx)


target_include_directories(connect PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(connect
        protobuf::libprotobuf
        Threads::Threads
        pqxx
)
