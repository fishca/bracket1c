cmake_minimum_required(VERSION 3.16)
project(1CBracketReader VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Опции для статической сборки
option(BUILD_STATIC "Build static executable" OFF)
option(QT_STATIC_BUILD "Use static Qt libraries" OFF)

if(BUILD_STATIC OR QT_STATIC_BUILD)
    # Статическая линковка для Qt
    set(BUILD_SHARED_LIBS OFF)
    
    # Для Windows - статическая линковка runtime
    if(MSVC)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
endif()

# Find Qt (Qt5 or Qt6)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/bracketparser.cpp
    src/treemodel.cpp
    src/treenode.cpp
)

set(HEADERS
    src/mainwindow.h
    src/bracketparser.h
    src/treemodel.h
    src/treenode.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Дополнительные библиотеки для статической сборки на Linux
if(LINUX AND BUILD_STATIC)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        pthread
        dl
        rt
    )
endif()

# Для macOS - фреймворки
if(APPLE)
    find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
    find_library(IOKIT_FRAMEWORK IOKit REQUIRED)
    find_library(COREVIDEO_FRAMEWORK CoreVideo REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        ${COCOA_FRAMEWORK}
        ${IOKIT_FRAMEWORK}
        ${COREVIDEO_FRAMEWORK}
    )
endif()
