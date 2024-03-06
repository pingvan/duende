cmake_minimum_required(VERSION 3.15)

project(duendeTests CXX)

set(SERVICES_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/database_tests/test.cpp
        PARENT_SCOPE
)