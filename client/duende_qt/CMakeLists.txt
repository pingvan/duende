cmake_minimum_required(VERSION 3.22.1)

project(duende LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(duende main.cpp duende.cpp duendedesktop.cpp duende.h duende.ui)

target_link_libraries(duende PRIVATE Qt${QT_VERSION}::Widgets)
