cmake_minimum_required(VERSION 3.16)
project(prompter VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Enable automatic MOC, UIC, and RCC processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
)

set(HEADERS
    src/MainWindow.h
)

# Create executable
add_executable(prompter ${SOURCES} ${HEADERS})

# Link Qt libraries
target_link_libraries(prompter
    Qt6::Core
    Qt6::Widgets
)

# Installation
install(TARGETS prompter
    RUNTIME DESTINATION bin
)
