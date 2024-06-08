cmake_minimum_required(VERSION 3.15)

project(duendeInclude CXX)

add_subdirectory(./components)
add_subdirectory(./services)

set(COMPONENTS_SRCS ${COMPONENTS_SRCS} PARENT_SCOPE)
set(SERVICES_SRCS ${SERVICES_SRCS} PARENT_SCOPE)