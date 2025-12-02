cmake_minimum_required(VERSION 3.17)
project(SysAdminGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find GLFW
find_package(glfw3 REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/includes)

# Source files
set(SOURCES
    src/main.cpp
    src/glad.c
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
