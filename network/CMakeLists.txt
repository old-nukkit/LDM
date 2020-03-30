project(ldm_network)

add_library(${PROJECT_NAME} "src/socket.cpp")

target_include_directories(${PROJECT_NAME} PRIVATE "include")

find_library(SSL ssl REQUIRED)
find_library(CRYPTO crypto REQUIRED)

target_link_libraries(${PROJECT_NAME} ${SSL} ${CRYPTO})
