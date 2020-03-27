cmake_minimum_required(VERSION 3.0)

project(module_http)

add_library(${PROJECT_NAME} "src/http.cpp")

target_include_directories(${PROJECT_NAME} PRIVATE "include")

find_library(MULTI_THREADING pthread REQUIRED)

target_link_libraries(${PROJECT_NAME} ${MULTI_THREADING} dl)