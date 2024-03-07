cmake_minimum_required(VERSION 3.22.1)

project(duende)

include(./cmake/grpc-generation.cmake)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(./include/)
add_subdirectory(./test/)

message(NOTICE "COMPONENTS_SRCS: " ${COMPONENTS_SRCS})
message(NOTICE "SERVICES_SRCS: " ${SERVICES_SRCS})

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

pkg_check_modules(LIBPQXX REQUIRED libpqxx)

#if error occurs visit https://cmake.org/cmake/help/latest/module/FindPkgConfig.html

add_executable(tests
       test/doctest_main.cpp

       ${TESTS_SRCS}

       ${COMPONENTS_SRCS}
       ${SERVICES_SRCS}

       ${generated_proto_models_srcs}
)

add_executable(main
    src/main.cpp

    ${COMPONENTS_SRCS}
    ${SERVICES_SRCS}

    ${generated_proto_models_srcs}
)

add_dependencies(main generate_proto_models)
#add_dependencies(test generate_proto_models)

target_link_libraries(main
        absl::flags_parse
        gRPC::grpc++
        protobuf::libprotobuf
        Threads::Threads
        pqxx
)

target_include_directories(main PUBLIC
        ./include
        ./libs
        ${CMAKE_CURRENT_BINARY_DIR}
        ${LIBPQXX_INCLUDE_DIRS}
)

target_link_options(main PUBLIC
        ${LIBPQXX_LDFLAGS})

target_link_libraries(tests
       absl::flags_parse
       gRPC::grpc++
       protobuf::libprotobuf
       Threads::Threads
       pqxx
)

target_include_directories(tests PUBLIC
       ./include
       ./libs
#        ${OPENSSL_CRYPTO_LIBRARY}
#        ${OPENSSL_INCLUDE_DIR}
       ${CMAKE_CURRENT_BINARY_DIR}
       ${LIBPQXX_INCLUDE_DIRS}
)

target_link_options(tests PUBLIC
       ${LIBPQXX_LDFLAGS})
