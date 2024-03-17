cmake_minimum_required(VERSION 3.15)

project(duendeComponents CXX)

set(COMPONENTS_SRCS
		${CMAKE_CURRENT_SOURCE_DIR}/database/services/authservice/authservice.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/database/services/profile_service/profile_service.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/database/services/chat_service/chat_service.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/jwt/jwt.cpp
		PARENT_SCOPE
)