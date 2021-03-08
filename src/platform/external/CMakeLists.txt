message(STATUS "GENERATING SDL")

set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)

add_subdirectory(sdl)

set_target_properties(SDL2main PROPERTIES FOLDER "External/SDL2")
set_target_properties(SDL2-static PROPERTIES FOLDER "External/SDL2")
set_target_properties(uninstall PROPERTIES FOLDER "External/SDL2")