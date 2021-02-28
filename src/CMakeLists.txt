if(WIN32)
    set(MAIN_FILE
        win32_main.cpp
    )
endif()

if(WIN32)
    add_executable(SimpleRenderer 
        WIN32
        ${MAIN_FILE}
        ${PLATFORM_HEADERS} ${PLATFORM_SOURCE_FILES}
    )
else()
    add_executable(SimpleRenderer 
        ${MAIN_FILE}
        ${PLATFORM_HEADERS} ${PLATFORM_SOURCE_FILES}
    )
endif()
set_target_properties(SimpleRenderer PROPERTIES FOLDER "Application")

add_subdirectory(platform)
target_link_libraries(SimpleRenderer Platform)