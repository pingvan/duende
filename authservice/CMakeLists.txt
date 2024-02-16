cmake_minimum_required(VERSION 3.22.1)

project(AuthService)

include_directories(include proto)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROTOS proto/authservice.grpc.pb.cc proto/authservice.pb.cc)

add_executable(authservice ${PROTOS} source/client.cpp source/client_main.cpp)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

target_include_directories(authservice PRIVATE
    ${OPENSSL_CRYPTO_LIBRARY}
    ${OPENSSL_INCLUDE_DIR}
    ${PROTOBUF_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    )

target_link_libraries(authservice
    Threads::Threads
    OpenSSL::Crypto
    protobuf::libprotobuf
    gRPC::grpc
    gRPC::grpc++
    )
        