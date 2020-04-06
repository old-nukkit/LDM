cmake_minimum_required(VERSION 3.10)

project(ldm_module-http CXX)

set(CMAKE_CXX_STANDARD 20)
set(SRC_FILES "./src/http.cpp")
set(HDR_FILES_DIR "./include")
set(LDM_NETWORK_HDR_DIR "../../network/include")
set(LDM_NETWORK_LIB ldm_network)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${HDR_FILES_DIR} ${LDM_NETWORK_HDR_DIR})
target_link_options(${PROJECT_NAME} PRIVATE "-rdynamic")
target_link_libraries(${PROJECT_NAME} ${LDM_NETWORK_LIB})

install(TARGETS ${PROJECT_NAME} DESTINATION "/usr/lib")
