cmake_minimum_required(VERSION 3.10)
project(asio_tcp_udp_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(Boost_INCLUDE_DIR "/home/obieliakov/boost/include")
set(Boost_LIBRARIES "/home/obieliakov/boost/lib")

find_package(Boost COMPONENTS thread system REQUIRED)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ./src/main.cpp)

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
