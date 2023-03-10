cmake_minimum_required(VERSION 3.21)
project(test_server_common VERSION 1.0.0)

set(src
	main.cpp
)

add_executable(${PROJECT_NAME}
	"${src}"
)

target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads ${Boost_LIBRARIES} CppKafka::cppkafka GTest::GTest GTest::Main common::lib server_common::lib)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

add_test(test-2 ${PROJECT_NAME})
