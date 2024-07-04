cmake_minimum_required(VERSION 3.16)

project(loglibs)

find_package(Boost 1.74 REQUIRED COMPONENTS log)
find_package(glog 0.6.0 REQUIRED)
find_package(spdlog REQUIRED)

include_directories(../..)

aux_source_directory(../src SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic -g -std=c++20")

target_link_libraries(${PROJECT_NAME} Boost::log glog::glog spdlog::spdlog)
