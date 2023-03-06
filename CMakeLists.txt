CMAKE_MINIMUM_REQUIRED(VERSION 3.21)

#find_program(CCACHE_PROGRAM ccache)
#if(CCACHE_PROGRAM)
#    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
#endif()

project(ClienServerEcn)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)
set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)

find_package(Boost 1.40 COMPONENTS system REQUIRED)
find_package(Threads REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_subdirectory(depends)
#find_package(CppKafka REQUIRED)

add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)


#add_executable(${PROJECT_NAME} main.cpp)
#target_link_libraries(${PROJECT_NAME] PUBLIC Threads::Threads ${Boost_LIBRARIES} common::lib})