cmake_minimum_required(VERSION 3.10)

project(ldm CXX)

set(CMAKE_CXX_STANDARD 20)
set(MAIN_CPP "./main.cpp")
set(LDM_CORE_HDRS_DIR "./core/include")
set(LDM_CORE_LIB ldm_core)

add_subdirectory("./modules")
add_subdirectory("./network")
add_subdirectory("./core")

add_executable(${PROJECT_NAME} ${MAIN_CPP})

target_include_directories(${PROJECT_NAME} PRIVATE ${LDM_CORE_HDRS_DIR})
target_link_libraries(${PROJECT_NAME} ${LDM_CORE_LIB})

install(TARGETS ${PROJECT_NAME} DESTINATION "/usr/bin")
