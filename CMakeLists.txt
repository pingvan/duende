cmake_minimum_required(VERSION 3.22.1)

project(AuthService)

include_directories(include proto libs)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROTOS proto/authservice.grpc.pb.cc proto/authservice.pb.cc)
set(CLIENT source/client/client.cpp source/client/client_main.cpp)
set(SERVER source/jwt.cpp source/data.cpp source/server/server.cpp source/server/server_main.cpp)

add_executable(client ${PROTOS} ${CLIENT})
add_executable(server ${PROTOS} ${SERVER})

add_custom_target(AuthService)
add_dependencies(AuthService
        client
        server
)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

target_include_directories(client PRIVATE
        ${OPENSSL_CRYPTO_LIBRARY}
        ${OPENSSL_INCLUDE_DIR}
        ${PROTOBUF_INCLUDE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        )

target_link_libraries(client
        Threads::Threads
        OpenSSL::Crypto
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
        )

target_include_directories(server PRIVATE
        ${OPENSSL_CRYPTO_LIBRARY}
        ${OPENSSL_INCLUDE_DIR}
        ${PROTOBUF_INCLUDE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(server
        Threads::Threads
        OpenSSL::Crypto
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
)
        