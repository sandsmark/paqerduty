cmake_minimum_required(VERSION 3.5)

project(paqerduty LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)
  add_executable(paqerduty
    main.cpp
    Notification.cpp
    Notification.h
    Poller.cpp
    Poller.h
    icon.qrc
  )

target_link_libraries(paqerduty PRIVATE Qt5::Widgets Qt5::Network)
