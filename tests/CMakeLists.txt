cmake_minimum_required(VERSION 3.15)

project(duendeTests CXX)

set(TESTS_SRCS 
    ${CMAKE_CURRENT_SOURCE_DIR}/placeholders/placeholders_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/database/database_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/integration/integration_test.cpp
    PARENT_SCOPE
)