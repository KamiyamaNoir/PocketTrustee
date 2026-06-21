cmake_minimum_required(VERSION 3.22)

add_library(cherryusb OBJECT)

target_link_libraries(cherryusb stm32cubemx)

target_include_directories(cherryusb
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/class/cdc/
        ${CMAKE_CURRENT_SOURCE_DIR}/class/hid/
        ${CMAKE_CURRENT_SOURCE_DIR}/common/
        ${CMAKE_CURRENT_SOURCE_DIR}/core/
        ${CMAKE_CURRENT_SOURCE_DIR}/port/fsdev/
)

target_sources(cherryusb
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/class/cdc/usbd_cdc_acm.c
        ${CMAKE_CURRENT_SOURCE_DIR}/class/hid/usbd_hid.c
        ${CMAKE_CURRENT_SOURCE_DIR}/core/usbd_core.c
        ${CMAKE_CURRENT_SOURCE_DIR}/port/fsdev/usb_glue_st.c
        ${CMAKE_CURRENT_SOURCE_DIR}/port/fsdev/usb_dc_fsdev.c
)