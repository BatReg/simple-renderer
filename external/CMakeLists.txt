message(STATUS "GENERATING SDL")

set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)

add_subdirectory(sdl)

target_include_directories(SimpleRenderer PRIVATE external/sdl/include)
target_link_libraries(SimpleRenderer SDL2main SDL2-static)

set_target_properties(SDL2main PROPERTIES FOLDER "SDL2")
set_target_properties(SDL2-static PROPERTIES FOLDER "SDL2")
set_target_properties(uninstall PROPERTIES FOLDER "SDL2")