project(Platform VERSION 0.0.1)

message(STATUS "GENERATING PLATFORM ABSTRACTION LAYER")

set(PLATFORM_INTERFACE include/platform/window.h
                       include/platform/vulkan.h)

set(PLATFORM_HEADERS src/internal_window.h)

if(WIN32)
    add_compile_definitions(PLATFORM_WIN32)

    set(PLATFORM_SOURCE src/win32/win32_platform.h
                        src/win32/win32_window.cpp
                        src/win32/win32_vulkan.cpp)
endif()

set(PLATFORM_SOURCE_FILES
    ${PLATFORM_HEADERS}
    ${PLATFORM_SOURCE}
)

source_group("include" FILES ${PLATFORM_INTERFACE})
source_group("src" FILES ${PLATFORM_SOURCE_FILES})

add_library(Platform STATIC ${PLATFORM_INTERFACE}
                            ${PLATFORM_SOURCE_FILES})
target_include_directories(Platform PUBLIC include)

add_subdirectory(external)
target_link_libraries(Platform PRIVATE SDL2main SDL2-static)

find_package(Vulkan REQUIRED)
target_link_libraries(Platform PRIVATE Vulkan::Vulkan)

set_target_properties(Platform PROPERTIES FOLDER "Platform Abstraction Layer")