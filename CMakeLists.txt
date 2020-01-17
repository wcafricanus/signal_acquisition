cmake_minimum_required(VERSION 3.0)

project(SignalAquisition)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
SET(CMAKE_PREFIX_PATH "~/Qt/5.12.6/clang_64/")


if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(SignalAquisition main.cpp mainwindow.cpp collector.cpp qcustomplot.cpp Utility/configuration.cpp Utility/savemanager.cpp Utility/SL_Data_linux.cpp Utility/TimerFunction.cpp)

target_include_directories(SignalAquisition
        PUBLIC .
        PUBLIC ADC
        PUBLIC Utility)

target_link_libraries(SignalAquisition -L/Users/wcafricanus/Downloads/singal_acquisition/lib Qt5::Widgets)
