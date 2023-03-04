cmake_minimum_required(VERSION 3.21)
project(order_status_machine VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)


set(headers
	include/order_status_machine/status_machine.hpp
)

set(src
	src/status_machine.cpp
)

add_library(${PROJECT_NAME} SHARED
	"${headers}"
	"${src}"
)
add_library(order_status_machine::lib ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads ${Boost_LIBRARIES} common::lib)
