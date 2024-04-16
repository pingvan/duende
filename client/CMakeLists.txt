cmake_minimum_required(VERSION 3.22.1)

project(duende LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

set(PROTOS ../build/services/authservice/authservice.grpc.pb.cc ../build/services/authservice/authservice.pb.cc ../build/services/chat_service/chat_service.grpc.pb.cc ../build/services/chat_service/chat_service.pb.cc ../build/services/profile_service/profile.grpc.pb.cc ../build/services/profile_service/profile.pb.cc)

qt6_add_resources(QRC_RESOURCES ui/resources.qrc)

add_executable(duende ${PROTOS} client.cpp ui/main.cpp ui/mainwindow.cpp ui/registrationpage.cpp ui/recommendations.cpp ui/chats.cpp ui/follows.cpp ui/profile.cpp ui/questionnaire.cpp ui/sidebar.cpp ui/mainwindow.h ui/mainwindow.ui ui/registrationpage.h ui/registrationpage.ui ui/recommendations.h ui/recommendations.ui ui/chats.h ui/chats.ui ui/follows.h ui/follows.ui ui/profile.h ui/profile.ui ui/questionnaire.h ui/questionnaire.ui ui/sidebar.h ui/sidebar.ui ui/actors_searchwindow.cpp ui/actors_searchwindow.h ui/actors_searchwindow.ui ui/movies_searchwindow.cpp ui/movies_searchwindow.h ui/movies_searchwindow.ui  ${QRC_RESOURCES})

target_include_directories(duende PRIVATE
    ${Qt6Widgets_INCLUDE_DIRS},
    ../build/
)

target_link_libraries(duende PRIVATE
    Qt${QT_VERSION}::Widgets
    absl::flags_parse
    gRPC::grpc++
    protobuf::libprotobuf
    Threads::Threads
    pqxx
    )
