CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
project(ClienServerEcn)

set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE(Boost 1.40 COMPONENTS system REQUIRED)
FIND_PACKAGE(Threads REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)

#add_executable(${PROJECT_NAME} main.cpp)
#target_link_libraries(${PROJECT_NAME] PUBLIC Threads::Threads ${Boost_LIBRARIES} common::lib})