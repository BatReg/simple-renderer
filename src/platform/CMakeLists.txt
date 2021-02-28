project(Platform VERSION 0.0.1)

message(STATUS "GENERATING PLATFORM ABSTRACTION LAYER")

set(PLATFORM_HEADERS
    include/platform/window.h
)

if(WIN32)
    set(PLATFORM_SOURCE_WINDOW
            win32/win32_window.cpp
        )
    
    set(PLATFORM_SOURCE_FILES
            ${PLATFORM_SOURCE_WINDOW}
        )
endif()

source_group("include" FILES ${PLATFORM_HEADERS})
source_group("src" FILES ${PLATFORM_SOURCE_FILES})

add_library(Platform STATIC 
    ${PLATFORM_HEADERS}
    ${PLATFORM_SOURCE_FILES}
)

target_include_directories(Platform PUBLIC
    include
)