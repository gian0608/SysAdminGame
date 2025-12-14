# SysAdmin Game - OpenGL Project

A 3D simulation game developed in C++ and OpenGL, where you play as a System Administrator maintaining a server room.

## Overview
This project demonstrates a 3D interactive application built from scratch using OpenGL. It features a custom engine implementation with procedural generation, hierarchical character animation, and a modern GUI overlay.

## Key Features

1.  **Build System**: Fully automated cross-platform build using **CMake**. Includes a helper script for easy compilation.
2.  **Environment**: 
    -   A texture-mapped 20x20 server room environment.
    -   Isometric camera view following the player.
3.  **Character (SysAdmin)**: 
    -   Complex hierarchical model (Head, Body, Arms, Legs).
    -   Procedural walking animation using trigonometric functions.
4.  **Gameplay**:
    -   **Procedural Content**: Servers are generated in a random 4x4 grid layout.
    -   **Collision System**: AABB (Axis-Aligned Bounding Box) collision detection prevents clipping through objects and walls.
    -   **Interaction**: Approach broken servers (red) and press Space to repair them.
5.  **Graphics**:
    -   Custom **Pixel Art** textures.
    -   `GL_NEAREST` filtering for a crisp retro aesthetic.
    -   **Dear ImGui** integration for a professional HUD and Menu system.

## Prerequisites

Before running the game, ensure you have the following installed:

### macOS (via Homebrew)
```bash
brew install cmake glfw
```

### Linux (Debian/Ubuntu)
```bash
sudo apt-get update
sudo apt-get install cmake libglfw3-dev
```

### Windows
Requires **CMake** and a C++ compiler (e.g., Visual Studio or MinGW). GLFW is handled via CMake (ensure binaries are available if not using Vcpkg/system install).

## How to Run

Simply execute the provided script:
```bash
./run.sh
```
**Controls**:
-   **W, A, S, D**: Move Character
-   **Space**: Repair Server (Interaction)
-   **ESC**: Quit / Menu

---

## Project Report: Compliance with Exam Requirements

This section details how the application satisfies the specific requirements for the Computer Graphics exam.

### 1. Topic Adherence
*Requirement: The application must adhere to the group project topic.*
-   **Compliance**: The **"SysAdmin Game"** is strictly related to the final project theme. It simulates a server room maintenance scenario.
-   **Reusability**: Core assets (textures, models) and logic (camera, grid movement) are modular and designed for integration into the final group delivery.

### 2. Playable Application
*Requirement: Implement a playable application with minimal logic.*
-   **Gameplay**: A complete loop including a Main Menu, active gameplay state, and exit functionality.
-   **Logic**: Implemented collision detection, game states (Menu/Game), and interaction logic (repairing servers).

### 3. Input Handling
*Requirement: Handling of mouse and keyboard events.*
-   **Keyboard**: Maps WASD for movement, Space for action, and ESC for system commands.
-   **Mouse**: Fully integrated with the ImGui menu for UI interactions (clicking buttons, navigating the interface).

### 4. Geometric Transformations & Animation
*Requirement: Using geometric transformations to animate contents.*
-   **Hierarchical Animation**: The character utilizes hierarchical transformations (parent-child relationships for limbs) animated via `sin(time)` to simulate realistic walking.
-   **Camera & World**: View and Projection matrices transformation for the isometric perspective; Translation/Rotation/Scaling matrices for object placement.

### 5. Simple Objects & Textures
*Requirement: Simple placeholders with textures are acceptable.*
-   **Implementation**: Primitive shapes (cubes) are combined to create complex objects.
-   **Texturing**: Every object is fully texture-mapped using custom-made pixel art assets loaded via `stb_image`.

