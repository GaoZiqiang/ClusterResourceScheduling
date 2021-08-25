cmake_minimum_required(VERSION 3.17)
project(TestRedis)

set(CMAKE_CXX_STANDARD 14)

add_executable(TestRedis main.cpp Headers/RedisConfig.h Headers/RedisTools.h Sources/RedisConfig.cpp Sources/RedisTools.cpp)
#add_executable(TestRedis main.cpp)