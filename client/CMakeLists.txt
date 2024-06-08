cmake_minimum_required(VERSION 3.22.1)

project(client LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

pkg_check_modules(LIBPQXX REQUIRED libpqxx)

set(COMMON_PROTO ../build/common/common.grpc.pb.cc ../build/common/common.pb.cc)
set(AUTHSERVICE_PROTO ../build/services/authservice/authservice.grpc.pb.cc ../build/services/authservice/authservice.pb.cc)
set(CHAT_SERVICE_PROTO ../build/services/chat_service/chat_service.grpc.pb.cc ../build/services/chat_service/chat_service.pb.cc)
set(PROFILE_SERVICE_PROTO ../build/services/profile_service/profile.grpc.pb.cc ../build/services/profile_service/profile.pb.cc)
set(SEARCH_SERVICE_PROTO ../build/services/search_service/search_service.grpc.pb.cc ../build/services/search_service/search_service.pb.cc)
set(INVITE_SERVICE_PROTO ../build/services/invitation_service/invitation.grpc.pb.cc ../build/services/invitation_service/invitation.pb.cc)
set(USERS_INTERACTION_SERVICE_PROTO ../build/services/users_interaction_service/users_interaction.grpc.pb.cc ../build/services/users_interaction_service/users_interaction.pb.cc)
set(RECOMMENDATIONS_SERVICE_PROTO ../build/services/recommendation_service/recommendation_service.grpc.pb.cc ../build/services/recommendation_service/recommendation_service.pb.cc)
set(PROTOS ${COMMON_PROTO} ${AUTHSERVICE_PROTO} ${CHAT_SERVICE_PROTO} ${PROFILE_SERVICE_PROTO} ${SEARCH_SERVICE_PROTO} ${INVITE_SERVICE_PROTO} ${USERS_INTERACTION_SERVICE_PROTO} ${RECOMMENDATIONS_SERVICE_PROTO})

set(MAINWINDOW ui/mainwindow/mainwindow.ui ui/mainwindow/mainwindow.h ui/mainwindow/mainwindow.cpp)
set(REGISTRATIONPAGE ui/registrationpage/registrationpage.ui ui/registrationpage/registrationpage.h ui/registrationpage/registrationpage.cpp)
set(RECOMMENDATIONS ui/recommendations/recommendations.ui ui/recommendations/recommendations.h ui/recommendations/recommendations.cpp)
set(CHATS ui/chats/chats.ui ui/chats/chats.h ui/chats/chats.cpp)
set(FOLLOWS ui/follows/follows.ui ui/follows/follows.h ui/follows/follows.cpp)
set(PROFILE ui/profile/profile.ui ui/profile/profile.h ui/profile/profile.cpp)
set(OTHERPAGE ui/otherpage/otherpage.ui ui/otherpage/otherpage.h ui/otherpage/otherpage.cpp)
set(QUESTIONNAIRE ui/questionnaire/questionnaire.ui ui/questionnaire/questionnaire.h ui/questionnaire/questionnaire.cpp)
set(ACTORS_SEARCHWINDOW ui/actors_searchwindow/actors_searchwindow.ui ui/actors_searchwindow/actors_searchwindow.h ui/actors_searchwindow/actors_searchwindow.cpp)
set(MOVIES_SEARCHWINDOW ui/movies_searchwindow/movies_searchwindow.ui ui/movies_searchwindow/movies_searchwindow.h ui/movies_searchwindow/movies_searchwindow.cpp)
set(GENRES_SEARCHWINDOW ui/genres_searchwindow/genres_searchwindow.ui ui/genres_searchwindow/genres_searchwindow.h ui/genres_searchwindow/genres_searchwindow.cpp)

qt6_add_resources(QRC_RESOURCES ui/resources.qrc)

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/ui)

add_executable(client ${PROTOS} client.cpp ui/manager.h ui/main.cpp ${MAINWINDOW} ${REGISTRATIONPAGE} ${RECOMMENDATIONS} ${CHATS} ${FOLLOWS} ${PROFILE} ${OTHERPAGE} ${QUESTIONNAIRE} ${ACTORS_SEARCHWINDOW} ${MOVIES_SEARCHWINDOW} ${GENRES_SEARCHWINDOW} ${QRC_RESOURCES})

target_include_directories(client PRIVATE
    ${Qt6Widgets_INCLUDE_DIRS},
    ../build/
    ${LIBPQXX_INCLUDE_DIRS}
)

target_link_libraries(client PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
    absl::flags_parse
    gRPC::grpc++
    protobuf::libprotobuf
    Threads::Threads
)

target_link_options(client PUBLIC
        ${LIBPQXX_LDFLAGS})
