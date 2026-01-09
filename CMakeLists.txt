cmake_minimum_required(VERSION 3.20)
project(GameTetris LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)        

# Find SDL2 packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Board.cpp
    src/Tetromino.cpp
    src/Renderer.cpp
    src/InputHandler.cpp
)

add_executable(game ${SOURCES})

target_link_libraries(game
    PRIVATE
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
)

target_include_directories(game
    PRIVATE
        ${CMAKE_SOURCE_DIR}/includes
)

add_custom_command(
    TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:game>/assets
    COMMENT "Copying assets to build directory..."
)
