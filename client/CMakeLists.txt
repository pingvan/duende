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

set(PROTOS ../build/services/authservice/authservice.grpc.pb.cc ../build/services/authservice/authservice.pb.cc ../build/services/chat_service/chat_service.grpc.pb.cc ../build/services/chat_service/chat_service.pb.cc ../build/services/profile_service/profile.grpc.pb.cc ../build/services/profile_service/profile.pb.cc ../build/services/search_service/search_service.grpc.pb.cc ../build/services/search_service/search_service.pb.cc)
set(MAINWINDOW ui/mainwindow/mainwindow.ui ui/mainwindow/mainwindow.h ui/mainwindow/mainwindow.cpp)
set(REGISTRATIONPAGE ui/registrationpage/registrationpage.ui ui/registrationpage/registrationpage.h ui/registrationpage/registrationpage.cpp)
set(RECOMMENDATIONS ui/recommendations/recommendations.ui ui/recommendations/recommendations.h ui/recommendations/recommendations.cpp)
set(CHATS ui/chats/chats.ui ui/chats/chats.h ui/chats/chats.cpp)
set(FOLLOWS ui/follows/follows.ui ui/follows/follows.h ui/follows/follows.cpp)
set(PROFILE ui/profile/profile.ui ui/profile/profile.h ui/profile/profile.cpp)
set(QUESTIONNAIRE ui/questionnaire/questionnaire.ui ui/questionnaire/questionnaire.h ui/questionnaire/questionnaire.cpp)
set(ACTORS_SEARCHWINDOW ui/actors_searchwindow/actors_searchwindow.ui ui/actors_searchwindow/actors_searchwindow.h ui/actors_searchwindow/actors_searchwindow.cpp)
set(MOVIES_SEARCHWINDOW ui/movies_searchwindow/movies_searchwindow.ui ui/movies_searchwindow/movies_searchwindow.h ui/movies_searchwindow/movies_searchwindow.cpp)
set(GENRES_SEARCHWINDOW ui/genres_searchwindow/genres_searchwindow.ui ui/genres_searchwindow/genres_searchwindow.h ui/genres_searchwindow/genres_searchwindow.cpp)

qt6_add_resources(QRC_RESOURCES ui/resources.qrc)

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/ui)

add_executable(duende ${PROTOS} client.cpp ui/main.cpp ${MAINWINDOW} ${REGISTRATIONPAGE} ${RECOMMENDATIONS} ${CHATS} ${FOLLOWS} ${PROFILE} ${QUESTIONNAIRE} ${ACTORS_SEARCHWINDOW} ${MOVIES_SEARCHWINDOW} ${GENRES_SEARCHWINDOW} ${QRC_RESOURCES})

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
