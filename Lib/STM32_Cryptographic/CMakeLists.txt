cmake_minimum_required(VERSION 3.22)

add_library(stm32cryptographic INTERFACE)

target_include_directories(stm32cryptographic INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/cmox_low_level.c)
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_link_libraries(${CMAKE_PROJECT_NAME} libSTM32Cryptographic_CM4.a)