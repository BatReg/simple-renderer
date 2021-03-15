project(Platform VERSION 0.0.1)

message(STATUS "GENERATING PLATFORM ABSTRACTION LAYER")

set(PLATFORM_HEADERS
    include/platform/window.h
)

set(PLATFORM_SOURCE_COMMON
    src/internal_window.h
)

if(WIN32)
    add_compile_definitions(PLATFORM_WIN32)

    set(PLATFORM_SOURCE_BASE
        src/win32/win32_platform.h
    )

    set(PLATFORM_SOURCE_WINDOW
        src/win32/win32_window.cpp
    )
endif()

set(PLATFORM_SOURCE_FILES
    ${PLATFORM_SOURCE_COMMON}
    ${PLATFORM_SOURCE_BASE}
    ${PLATFORM_SOURCE_WINDOW}
)

source_group("include" FILES ${PLATFORM_HEADERS})
source_group("src" FILES ${PLATFORM_SOURCE_FILES})

add_library(Platform STATIC 
    ${PLATFORM_HEADERS}
    ${PLATFORM_SOURCE_FILES}
)

target_include_directories(Platform PUBLIC include)

add_subdirectory(external)
target_link_libraries(Platform PRIVATE SDL2main SDL2-static)

set_target_properties(Platform PROPERTIES FOLDER "Platform Abstraction Layer")