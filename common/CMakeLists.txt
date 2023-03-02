cmake_minimum_required(VERSION 3.21)
project(common)

add_library(${PROJECT_NAME} SHARED
	${PROJECT_SOURCE_DIR}/include/common/client.hpp
	${PROJECT_SOURCE_DIR}/include/common/Common.hpp
	${PROJECT_SOURCE_DIR}/include/common/db_connector.hpp
	${PROJECT_SOURCE_DIR}/include/common/json.hpp
	${PROJECT_SOURCE_DIR}/include/common/order.hpp
	${PROJECT_SOURCE_DIR}/include/common/orderbook.cpp
	${PROJECT_SOURCE_DIR}/include/common/orderbook.hpp
	${PROJECT_SOURCE_DIR}/include/common/volume_converter.cpp
	${PROJECT_SOURCE_DIR}/include/common/volume_converter.hpp
	${PROJECT_SOURCE_DIR}/include/common/web_connector.hpp
)
add_library(common::lib ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
