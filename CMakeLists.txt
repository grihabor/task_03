cmake_minimum_required(VERSION 3.0)
project(task_03)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("Utility/include"
                    "Utility/include/GL"
                    "Utility/include/VM")

set(SOURCE_FILES
    "Simple OpenGL Image Library/src/image_DXT.c"
    "Simple OpenGL Image Library/src/image_helper.c"
    "Simple OpenGL Image Library/src/SOIL.c"
    "Simple OpenGL Image Library/src/stb_image_aug.c"
    Utility/src/Camera.cpp
    Utility/src/GL.cpp
    Utility/src/mat4.cpp
    Utility/src/Overall.cpp
    Utility/src/ShaderProgram.cpp
    Utility/src/Texture.cpp
    Utility/src/uvec3.cpp
    Utility/src/vec2.cpp
    Utility/src/vec3.cpp
    Utility/src/vec4.cpp
    main.cpp)



add_executable(task_03 ${SOURCE_FILES})
target_link_libraries(task_03 libGL.so libglut.so libGLEW.so libGLU.so)

add_custom_target(textures ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Texture $<TARGET_FILE_DIR:task_03>/Texture)
      
add_custom_target(shaders ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:task_03>/shaders)

add_custom_target(program DEPENDS shaders textures)
