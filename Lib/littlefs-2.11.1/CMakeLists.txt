cmake_minimum_required(VERSION 3.22)

add_library(littlefs OBJECT)

target_include_directories(littlefs
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(littlefs
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/lfs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lfs_util.c
)
