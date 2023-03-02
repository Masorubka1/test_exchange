cmake_minimum_required(VERSION 3.21)
project(client)

add_executable(${PROJECT_NAME} Client.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads ${Boost_LIBRARIES} common::lib)