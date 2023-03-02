cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(server_common)

add_library(${PROJECT_NAME} SHARED
	${PROJECT_SOURCE_DIR}/include/server_common/clients.cpp
	${PROJECT_SOURCE_DIR}/include/server_common/clients.hpp
	${PROJECT_SOURCE_DIR}/include/server_common/matcher.cpp
	${PROJECT_SOURCE_DIR}/include/server_common/matcher.hpp
	${PROJECT_SOURCE_DIR}/include/server_common/server_connector.cpp
	${PROJECT_SOURCE_DIR}/include/server_common/server_connector.hpp
	${PROJECT_SOURCE_DIR}/include/server_common/timer.hpp
)
add_library(server_common::lib ALIAS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads ${Boost_LIBRARIES} common::lib)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)