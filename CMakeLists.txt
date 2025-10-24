cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# source code
file(GLOB_RECURSE BSP_SOURCE "./BSP/Src/*.cpp")
file(GLOB_RECURSE APP_SOURCE "./Apps/Src/*.cpp")
file(GLOB_RECURSE GUI_SOURCE "./GUI/Src/*.cpp")

# Set the project name
set(CMAKE_PROJECT_NAME PocketTrustee)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C CXX ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
    ./Lib/STM32_Cryptographic/lib/
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${BSP_SOURCE}
    ${APP_SOURCE}
    ${GUI_SOURCE}
    ${ZCBOR_FILE}
    # lib source
    # LittleFS
    ./Lib/littlefs-2.11.1/lfs.c
    ./Lib/littlefs-2.11.1/lfs_util.c
    # CherryUSB
    ./Lib/CherryUSB-1.5.2/class/cdc/usbd_cdc_acm.c
    ./Lib/CherryUSB-1.5.2/class/hid/usbd_hid.c
    ./Lib/CherryUSB-1.5.2/core/usbd_core.c
    ./Lib/CherryUSB-1.5.2/port/fsdev/usb_glue_st.c
    ./Lib/CherryUSB-1.5.2/port/fsdev/usb_dc_fsdev.c
    # zcbor
    ./Lib/zcbor-0.9.1/src/zcbor_common.c
    ./Lib/zcbor-0.9.1/src/zcbor_decode.c
    ./Lib/zcbor-0.9.1/src/zcbor_encode.c
    ./Lib/zcbor-0.9.1/src/zcbor_print.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    BSP/Inc
    Apps/Inc
    # lib include file
    # LittleFS
    ./Lib/littlefs-2.11.1/
    # STM32 Cryptographic
    ./Lib/STM32_Cryptographic/include/
    # CherryUSB
    ./Lib/CherryUSB-1.5.2/class/cdc/
    ./Lib/CherryUSB-1.5.2/class/hid/
    ./Lib/CherryUSB-1.5.2/common/
    ./Lib/CherryUSB-1.5.2/core/
    ./Lib/CherryUSB-1.5.2/port/fsdev/
    # zcbor
    ./Lib/zcbor-0.9.1/include/
    ./zcbor_generate/Inc/
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
    libSTM32Cryptographic_CM4.a
)
