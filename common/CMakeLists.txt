cmake_minimum_required(VERSION 3.21)
project(common VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)


set(headers
	include/common/client.hpp
	include/common/Common.hpp
	include/common/db_connector.hpp
	include/common/json.hpp
	include/common/order.hpp
	include/common/orderbook.hpp
	include/common/volume_converter.hpp
	include/common/web_connector.hpp
)

set(src
	src/orderbook.cpp
	src/volume_converter.cpp
)

add_library(${PROJECT_NAME} SHARED
	"${headers}"
	"${src}"
)
add_library(common::lib ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads ${Boost_LIBRARIES} CppKafka::cppkafka)
