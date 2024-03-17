cmake_minimum_required(VERSION 3.22.1)

project(duende LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

set(PROTOS ../../build/services/authservice/authservice.grpc.pb.cc ../../build/services/authservice/authservice.pb.cc)

add_executable(duende ${PROTOS} ../client.cpp main.cpp mainwindow.cpp registrationpage.cpp recommendations.cpp chats.cpp follows.cpp profile.cpp questionnaire.cpp sidebar.cpp mainwindow.h mainwindow.ui registrationpage.h registrationpage.ui recommendations.h recommendations.ui chats.h chats.ui follows.h follows.ui profile.h profile.ui questionnaire.h questionnaire.ui sidebar.h sidebar.ui)

target_include_directories(duende PRIVATE
    ${Qt6Widgets_INCLUDE_DIRS},
    ../../build/
)

target_link_libraries(duende PRIVATE
    Qt${QT_VERSION}::Widgets
    absl::flags_parse
    gRPC::grpc++
    protobuf::libprotobuf
    Threads::Threads
    pqxx
    )
