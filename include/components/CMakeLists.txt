cmake_minimum_required(VERSION 3.15)

project(duendeComponents CXX)

set(COMPONENTS_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/jwt/jwt.cpp
	PARENT_SCOPE
)