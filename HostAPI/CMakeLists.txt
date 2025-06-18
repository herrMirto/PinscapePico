cmake_minimum_required(VERSION 3.5)
project(PinscapePicoHostAPI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)
# Detect platform
if(APPLE)
    message(STATUS "Building on macOS - using hidapi (IOKit backend)")
    pkg_check_modules(HIDAPI REQUIRED hidapi)
elseif(UNIX)
    message(STATUS "Building on Linux - using hidapi-hidraw (hidraw backend)")
    pkg_check_modules(HIDAPI REQUIRED hidapi-hidraw)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()
#pkg_check_modules(HIDAPI REQUIRED hidapi-hidraw)

add_library(PinscapePicoHostAPI PinscapePicoAPI.cpp)

target_include_directories(PinscapePicoHostAPI PUBLIC 
    ${LIBUSB_INCLUDE_DIRS}
    ${HIDAPI_INCLUDE_DIRS}
)

target_link_libraries(PinscapePicoHostAPI 
    ${LIBUSB_LIBRARIES}
    ${HIDAPI_LIBRARIES}
)

