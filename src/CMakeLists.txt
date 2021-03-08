if(WIN32)
    set(MAIN_FILE
        win32_main.cpp
    )
endif()

add_executable(SimpleRenderer 
    ${MAIN_FILE}
)

add_subdirectory(platform)
target_link_libraries(SimpleRenderer Platform)

add_subdirectory(renderer)
target_link_libraries(SimpleRenderer Renderer)

set_target_properties(SimpleRenderer PROPERTIES FOLDER "Application")