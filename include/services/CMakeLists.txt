cmake_minimum_required(VERSION 3.15)

project(duendeUtils CXX)

set(SERVICES_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/authservice/service.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/authservice/token.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/authservice/data.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/authservice/hash.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/authservice/validity.cpp
	PARENT_SCOPE
)