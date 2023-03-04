cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
project(exchange_machine)

#add_executable(${PROJECT_NAME} Server.cpp)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads ${Boost_LIBRARIES} common::lib server_common::lib order_status_machine::lib)