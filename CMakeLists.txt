CMAKE_MINIMUM_REQUIRED(VERSION 3.21)

project(ClienServerEcn)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
#set(CMAKE_CXX_CPPLINT "cpplint")
add_compile_options(-fno-omit-frame-pointer -fsanitize=address)
add_link_options(-fno-omit-frame-pointer -fsanitize=address)
#set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)
set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)

enable_testing()
find_package(Boost 1.40 COMPONENTS system REQUIRED)
find_package(Threads REQUIRED)
find_package(CppKafka REQUIRED)
find_package(GTest REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
include_directories(${GTEST_INCLUDE_DIR})


add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(tests)
