cmake_minimum_required(VERSION 3.5)

project(box2dx)

file(GLOB SOURCES "src/common/*.cpp" "src/collision/*.cpp" "src/collision/shapes/*.cpp" "src/dynamics/*.cpp" "src/dynamics/contacts/*.cpp" "src/dynamics/joints/*.cpp" "src/rope/*.cpp")

add_library(box2dx STATIC ${SOURCES})

target_include_directories(box2dx PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)