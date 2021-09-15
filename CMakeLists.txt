cmake_minimum_required(VERSION 3.17)
project(ResourceSchedule)

set(CMAKE_CXX_STANDARD 14)

# 只需将源代码文件加入到项目中，而不用加入头文件--src/calculate_balance/calculate_balance.h src/resource_schedule/schedule.h
set(SOURCE_FILES src/main.cpp src/calculate_balance/calculate_balance.cpp
        src/resource_schedule/schedule.cpp)
#问题：为什么不需要加上这个utils/resource_free_utils.h?
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

find_Package(Threads)
target_link_libraries(${PROJECT_NAME} Threads::Threads)

# 对hiredis本地依赖的注入
#find_Package(redis)
#target_link_libraries(${PROJECT_NAME} /usr/include/redis)
#add_executable(ResourceSchedule utils/test_main.cpp)