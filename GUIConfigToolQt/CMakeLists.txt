cmake_minimum_required(VERSION 3.5)
project(PinscapePicoGUI LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(PinscapePicoGUI
    main.cpp
    MainWindow.cpp
)

target_link_libraries(PinscapePicoGUI Qt6::Widgets)
