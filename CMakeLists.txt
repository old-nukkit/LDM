cmake_minimum_required(VERSION 3.10)

project(LDM)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory("core")
add_subdirectory("network")
add_subdirectory("modules")

set(LDM_CORE ldm_core)
set(LDM_NET ldm_network)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC "core/include")

target_link_libraries(${PROJECT_NAME} ${LDM_CORE} ${LDM_NET})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
