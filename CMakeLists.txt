cmake_minimum_required(VERSION 3.17)
project(SysAdminGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find GLFW
find_package(glfw3 REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/includes)
include_directories(${CMAKE_SOURCE_DIR}/includes/imgui)

# Source files
set(SOURCES
    src/main.cpp
    src/glad.c
    src/imgui/imgui.cpp
    src/imgui/imgui_demo.cpp
    src/imgui/imgui_draw.cpp
    src/imgui/imgui_tables.cpp
    src/imgui/imgui_widgets.cpp
    src/imgui/imgui_impl_glfw.cpp
    src/imgui/imgui_impl_opengl3.cpp
)

# Executable
add_executable(SysAdminGame ${SOURCES})

# Link libraries
# We link GLFW and the system OpenGL libraries
if(APPLE)
    target_link_libraries(SysAdminGame glfw "-framework OpenGL" "-framework IOKit" "-framework Cocoa" "-framework CoreVideo")
else()
    target_link_libraries(SysAdminGame glfw3 GL dl)
endif()
