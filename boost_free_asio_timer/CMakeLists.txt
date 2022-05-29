cmake_minimum_required(VERSION 3.10)
project(boost_free_asio_timer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

INCLUDE_DIRECTORIES("/home/obieliakov/asio/include")

add_executable(${PROJECT_NAME} ./src/main.cpp)
