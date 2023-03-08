cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
project(server_common VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)


set(headers
	include/server_common/clients.hpp
	include/server_common/matcher.hpp
	include/server_common/server_connector.hpp
	include/server_common/timer.hpp
)

set(src
	src/clients.cpp
	src/matcher.cpp
	src/server_connector.cpp
)

add_library(${PROJECT_NAME} SHARED
	"${headers}"
	"${src}"
)

add_library(server_common::lib ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads ${Boost_LIBRARIES} CppKafka::cppkafka common::lib order_status_machine::lib)