project(space_arena)

cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig REQUIRED)

find_package(OpenGL REQUIRED)

include_directories("/usr/include/bullet")

SET(GXX_USE_CPP11_STANDARD "-std=c++11")
#SET(GXX_INSERT_DEBUGGING_SYMBOLS "-g")
set(CMAKE_BUILD_TYPE "Debug")
SET(SOIL_SRCS
        SOIL/image_helper.c
        SOIL/stb_image_aug.c
        SOIL/image_DXT.c
        SOIL/SOIL.c
)
#add_definitions(${GXX_USE_CPP11_STANDARD} ${GXX_INSERT_DEBUGGING_SYMBOLS})
add_definitions(${GXX_USE_CPP11_STANDARD})

add_executable(space_arena
        common/controls.cpp
        common/objloader.cpp
        common/quaternion_utils.cpp
        common/shader.cpp
        common/tangentspace.cpp
        common/text2D.cpp
        common/texture.cpp
        common/vboindexer.cpp
        common/lodepng.cpp

        core/game_sprite.cpp
        core/game_camera.cpp
        core/game_object.cpp
        core/game_player.cpp
        core/game_skybox.cpp
        core/game_shader.cpp
        core/game_physics.cpp
        core/game_engine.cpp
        core/gl_err_log.cpp

        shaders/ModelViewProjection.vertexshader
        shaders/UvMappedTexture.fragmentshader

        main.cpp
)

target_link_libraries(space_arena
        ${OPENGL_LIBRARY}
        glfw
        GLEW
        SOIL
        BulletSoftBody
        BulletDynamics
        BulletCollision
        LinearMath
)

install(TARGETS space_arena RUNTIME DESTINATION bin)
