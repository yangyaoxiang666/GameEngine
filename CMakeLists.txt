cmake_minimum_required(VERSION 3.10)
project(MyEngine)

set(CMAKE_CXX_STANDARD 17)

# 添加 GLFW
add_subdirectory(thirdparty/glfw)

# 头文件搜索路径
include_directories(
    src
    thirdparty/glfw/include
    thirdparty/glad/include
)

# 明确列出源文件
set(SOURCES
    src/main.cpp
    src/Core/Window.cpp
    src/Renderer/Shader.cpp
)

# GLAD 静态库
add_library(glad thirdparty/glad/src/glad.c)

# 可执行文件
add_executable(MyEngine ${SOURCES})

# 链接库
target_link_libraries(MyEngine glfw glad)

if (WIN32)
    target_link_libraries(MyEngine opengl32)
else()
    target_link_libraries(MyEngine GL)
endif()
