cmake_minimum_required(VERSION 3.10)
project(WeatherAPI)

set(CMAKE_CXX_STANDARD 11)

find_package(CURL REQUIRED)
find_package(jsoncpp REQUIRED)

add_executable(weatherapi main.cpp)
target_link_libraries(weatherapi CURL::libcurl jsoncpp_lib)
