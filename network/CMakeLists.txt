cmake_minimum_required(VERSION 3.10)

project(ldm_network CXX)

set(CMAKE_CXX_STANDARD 20)
set(SRC_FILES "./src/socket.cpp")
set(HDR_FILES_DIR "./include")

find_library(SSL ssl REQUIRED)
find_library(CRYPTO crypto REQUIRED)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${HDR_FILES_DIR})
target_link_libraries(${PROJECT_NAME} ${SSL} ${CRYPTO})

install(TARGETS ${PROJECT_NAME} DESTINATION "/usr/lib")
