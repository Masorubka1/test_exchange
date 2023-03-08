cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
project(exchange_machine)

set(headers
	exchange.hpp
)

set(src
    exchange.cpp
    main.cpp
)

#add_executable(${PROJECT_NAME} Server.cpp)
add_executable(${PROJECT_NAME} 
    "${headers}"
    "${src}"
)

target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads ${Boost_LIBRARIES} CppKafka::cppkafka common::lib server_common::lib order_status_machine::lib)