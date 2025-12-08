cmake_minimum_required(VERSION 3.15)
project(Projet C)

# Utilise C99 ou plus récent
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# --- Sources ---
set(SOURCES
    src/main.c
    src/button.c
    src/map.c 
    src/direction.c
    src/game_displayer.c
    src/menu_displayer.c
    src/window.c
)

# --- Include directories ---
include_directories(include)

# --- SDL3 ---
# Essaie de trouver SDL3 via pkg-config ou le finder intégré
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

# --- Exécutable ---
add_executable(programme ${SOURCES})

# --- Liens ---
target_link_libraries(programme
    SDL3::SDL3
    SDL3_image::SDL3_image
)

# --- Spécifique à macOS (optionnel) ---
if(APPLE)
    message(STATUS "Configuration macOS : ajout de chemins Homebrew")
    include_directories(/opt/homebrew/include)
    link_directories(/opt/homebrew/lib)
endif()

# --- Commande "make run" équivalente ---
add_custom_target(run
    COMMAND programme
    DEPENDS programme
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_command(
    TARGET programme POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            ${CMAKE_BINARY_DIR}/assets
)
