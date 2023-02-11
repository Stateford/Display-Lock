cmake_minimum_required(VERSION 3.24)

project(Display-Lock)

add_subdirectory(src)
target_include_directories(${PROJECT_NAME} PRIVATE include)
