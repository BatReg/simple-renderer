cmake_minimum_required(VERSION 3.13.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(SimpleRenderer VERSION 0.0.1)

set(PLATFORM_HEADERS 
    include/platform/window.h
)

if(WIN32)
    set(PLATFORM_SOURCE_FILES 
        src/platform/win32/win32_window.cpp
    )
endif()

source_group("src\\platform\\include" FILES ${PLATFORM_HEADERS})
source_group("src\\platform\\src" FILES ${PLATFORM_SOURCE_FILES})

if(WIN32)
    set(MAIN_FILE
        src/win32_main.cpp
    )
endif()

source_group("src" FILES ${MAIN_FILE})

add_executable(SimpleRenderer WIN32 ${MAIN_FILE}
    ${PLATFORM_HEADERS} ${PLATFORM_SOURCE_FILES})
target_include_directories(SimpleRenderer PRIVATE include)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SimpleRenderer)