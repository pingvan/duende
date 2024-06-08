cmake_minimum_required(VERSION 3.22.1)

project(duende)

include(./cmake/grpc-generation.cmake)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_CXX_FLAGS "-Wall -Wextra")
# set(CMAKE_CXX_FLAGS "-fsanitize=undefined,address")

add_subdirectory(./include/)
add_subdirectory(./tests/ build-tests)
add_subdirectory(./libs/libpqxx build-pqxx)

message(NOTICE "COMPONENTS_SRCS: " ${COMPONENTS_SRCS})
message(NOTICE "SERVICES_SRCS: " ${SERVICES_SRCS})
message(NOTICE "TESTS_SRCS: " ${TESTS_SRCS})

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(GTest REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(main
    src/main.cpp

    ${COMPONENTS_SRCS}
    ${SERVICES_SRCS}

    ${generated_proto_models_srcs}
)

add_executable(tests
        tests/test_main.cpp

        ${TESTS_SRCS}

        ${COMPONENTS_SRCS}

        ${generated_proto_models_srcs}
)


add_dependencies(main generate_proto_models)
add_dependencies(main pqxx)
add_dependencies(tests generate_proto_models)
add_dependencies(tests pqxx)


target_link_libraries(main
        absl::flags_parse
        gRPC::grpc++
        protobuf::libprotobuf
        Threads::Threads
        pqxx
        OpenSSL::SSL
        OpenSSL::Crypto
)

target_include_directories(main PUBLIC
        ./include
        ./libs
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(tests
        absl::flags_parse
        gRPC::grpc++
        protobuf::libprotobuf
        Threads::Threads
        GTest::gtest_main 
        GTest::gmock_main
        pqxx
)

target_include_directories(tests PUBLIC
        ./include
        ./libs
        ${CMAKE_CURRENT_BINARY_DIR}
)