cmake_minimum_required(VERSION 3.15)
project(Projet C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# --- Sources ---
set(SOURCES
    src/button.c
    src/game_displayer.c
    src/main.c
    src/menu_displayer.c
    src/parking.c 
    src/vehicule.c
    src/window.c
)

# --- Includes ---
include_directories(include)

# --- SDL3 ---
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)

# --- Exécutable ---
add_executable(programme ${SOURCES})

# --- Liens ---
target_link_libraries(programme
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)

# --- macOS (optionnel) ---
if(APPLE)
    include_directories(/opt/homebrew/include)
    link_directories(/opt/homebrew/lib)
endif()

# --- Copier assets ---
add_custom_command(
    TARGET programme POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            ${CMAKE_BINARY_DIR}/assets
)
