if(WIN32)
    set(MAIN_FILE
        win32_main.cpp
    )
endif()

if(WIN32)
    add_executable(SimpleRenderer 
        WIN32
        ${MAIN_FILE}
    )
else()
    add_executable(SimpleRenderer 
        ${MAIN_FILE}
    )
endif()

add_subdirectory(platform)
target_link_libraries(SimpleRenderer Platform)

add_subdirectory(renderer)
target_link_libraries(SimpleRenderer Renderer)

set_target_properties(SimpleRenderer PROPERTIES FOLDER "Application")