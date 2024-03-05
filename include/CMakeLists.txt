cmake_minimum_required(VERSION 3.15)

project(duendeInclude CXX)

# adding subdirs
add_subdirectory(./components)
add_subdirectory(./services)

# propagating variable from inner subdirs to root level project
set(COMPONENTS_SRCS ${COMPONENTS_SRCS} PARENT_SCOPE)
set(SERVICES_SRCS ${SERVICES_SRCS} PARENT_SCOPE)
