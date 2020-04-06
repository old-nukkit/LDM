cmake_minimum_required(VERSION 3.10)

project(ldm_core)

set(CMAKE_CXX_STANDARD 20)
set(SRC_FILES "./src/core.cpp")
set(HDR_FILES_DIR "./include")

find_library(DL dl REQUIRED)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${HDR_FILES_DIR})
target_link_libraries(${PROJECT_NAME} ${DL})

install(TARGETS ${PROJECT_NAME} DESTINATION "/usr/lib")
