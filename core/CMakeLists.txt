project(ldm_core)

add_library(${PROJECT_NAME} "src/core.cpp")

target_include_directories(${PROJECT_NAME} PRIVATE "include")

target_link_libraries(${PROJECT_NAME})