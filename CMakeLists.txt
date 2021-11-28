cmake_minimum_required(VERSION 3.21.3)

project(minesweeper)

set(EXECUTABLE_PATH "bin/x64")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_PATH}/${CMAKE_BUILD_TYPE}")

# Only apply these if a unix system
if(UNIX)
    add_compile_options(
        -Wall
        -lpthread
        
        $<$<CONFIG:Release>:-O2>
        $<$<CONFIG:Debug>:-O0>
    )

    add_link_options(
        $<$<CONFIG:Release>:-s>
    )
endif()

# The include directories
include_directories(
    src/
    include/
    vendor/glfw/include/
    vendor/glm/glm/
    vendor/Random/RandomClass/src/
    vendor/Timer/Timer/src/
    vendor/glad/include/
    vendor/stb_image/
)

# Adding libraries
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glm)
add_library(timer-lib
    vendor/Timer/Timer/src/Timer.cpp
)
add_library(random-lib
    vendor/Random/RandomClass/src/Random.cpp
)
add_library(glad-lib
    vendor/glad/src/glad.c
)
add_library(stbi-lib
    vendor/stb_image/stb_image.cpp
)

# Add executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/renderer/renderer.cpp
    src/renderer/IndexBuffer.cpp
    src/renderer/VertexArray.cpp
    src/renderer/VertexBuffer.cpp
    src/renderer/VertexBufferLayout.cpp
    src/Window.cpp
    src/renderer/Shader.cpp
    src/minesweeper.cpp
    src/board.cpp
)

# Linking libraries
target_link_directories(${PROJECT_NAME} PUBLIC
    vendor/glfw/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glm
    timer-lib
    random-lib
    glad-lib
    stbi-lib
)


configure_file(${CMAKE_SOURCE_DIR}/assets/shaders/vertex.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/shaders/vertex.glsl COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/shaders/fragment.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/shaders/fragment.glsl COPYONLY)

configure_file(${CMAKE_SOURCE_DIR}/assets/textures/tiles/empty.png ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures/tiles/empty.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/textures/tiles/square1.png ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures/tiles/square1.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/textures/tiles/square2.png ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures/tiles/square2.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/textures/tiles/square3.png ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures/tiles/square3.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/textures/tiles/square4.png ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures/tiles/square4.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/textures/tiles/square5.png ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures/tiles/square5.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/textures/tiles/square6.png ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures/tiles/square6.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/textures/tiles/square7.png ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures/tiles/square7.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/textures/tiles/square8.png ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures/tiles/square8.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/textures/tiles/squarebomb.png ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures/tiles/squarebomb.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/textures/tiles/squarebombred.png ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures/tiles/squarebombred.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/textures/tiles/squareflag.png ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures/tiles/squareflag.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/textures/tiles/unclicked.png ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures/tiles/unclicked.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/textures/tiles/squareflagwrong.png ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures/tiles/squareflagwrong.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/textures/winner.png ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures/winner.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/textures/bird.png ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures/bird.png COPYONLY)