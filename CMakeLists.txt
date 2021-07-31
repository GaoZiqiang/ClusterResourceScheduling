cmake_minimum_required(VERSION 3.17)
project(ResourceSchedule)

set(CMAKE_CXX_STANDARD 14)


set(SOURCE_FILES src/main.cpp src/calculate_balance/calculate_balance.h src/calculate_balance/calculate_balance.cpp src/resource_schedule/schedule.h src/resource_schedule/schedule.cpp)
#问题：为什么不需要加上这个utils/resource_free_utils.h?
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

find_Package(Threads)
target_link_libraries(${PROJECT_NAME} Threads::Threads)
#add_executable(ResourceSchedule utils/test_main.cpp)