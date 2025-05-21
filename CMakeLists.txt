cmake_minimum_required(VERSION 3.10)

project(snake VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/../bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/../bin/release)

include_directories(
    ${CMAKE_SOURCE_DIR}/external/include
    ${CMAKE_SOURCE_DIR}/src/include
)
link_directories(${CMAKE_SOURCE_DIR}/external/lib)


set(PROJECT_SOURCE
    src/source/game.cpp
    src/source/Game/Map.cpp
    src/source/Utils/Logger.cpp
    src/source/Core/Shader.cpp
    src/source/Utils/Utils.cpp
    src/source/Utils/OpenGLErrors.cpp
)

add_executable(${PROJECT_NAME} 
    ${PROJECT_SOURCE}
)

# target_include_directories(${PROJECT_NAME} 
# PRIVATE
# ${CMAKE_SOURCE_DIR}/external/include
# )

if(WIN32) 
    target_link_libraries(${PROJECT_NAME} PRIVATE
        glew32
        glfw3
        glu32
        opengl32
    )
else()
    find_package(OpenGL REQUIRED)
    find_package(pkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
    pkg_check_modules(GLEW REQUIRED glew)

    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${OPENGL_LIBRARIES}
        ${GLFW_LIBRARIES}
        ${GLEW_LIBRARIES}
    )
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${OPENGL_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
    )
endif()

# 如果使用 CMake 3.13+，可以尝试 target_link_directories（但通常不必要）
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.13")
    target_link_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/external/lib
    )
endif()