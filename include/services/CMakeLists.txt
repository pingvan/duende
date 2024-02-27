cmake_minimum_required(VERSION 3.15)

project(duendeUtils CXX)

set(SERVICES_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/authservice/server.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/authservice/data.cpp
	PARENT_SCOPE
)