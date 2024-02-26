cmake_minimum_required(VERSION 3.25)
project(grpc_chat LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

# set(OPENSSL_ROOT_DIR /opt/homebrew/Cellar/openssl@3/3.2.1)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

add_executable(profile_client profile_client.cpp profile.grpc.pb.cc profile.pb.cc)
add_executable(profile_server profile_server.cpp profile.grpc.pb.cc profile.pb.cc)

target_include_directories(profile_server PRIVATE
        ${OPENSSL_CRYPTO_LIBRARY}
        ${OPENSSL_INCLUDE_DIR}
        ${PROTOBUF_INCLUDE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        )

        target_include_directories(profile_client PRIVATE
        ${OPENSSL_CRYPTO_LIBRARY}
        ${OPENSSL_INCLUDE_DIR}
        ${PROTOBUF_INCLUDE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        )

target_link_libraries(profile_server
        Threads::Threads
        OpenSSL::Crypto
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
        )

        target_link_libraries(profile_client
        Threads::Threads
        OpenSSL::Crypto
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
        )