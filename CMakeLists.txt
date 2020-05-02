cmake_minimum_required(VERSION 3.15)
project(meat-wagons)

set(CMAKE_CXX_STANDARD 14)
link_libraries(ws2_32 wsock32)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# The line below is necessary if you are under Windows only
# Comment the line below if you are under Linux or Mac OS
file(GLOB_RECURSE SRC "src/*.cpp" "src/*.h")
add_executable(meat-wagons ${SRC})
