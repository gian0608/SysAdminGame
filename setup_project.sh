#!/bin/bash

# Create directories
mkdir -p src
mkdir -p includes
mkdir -p includes/glad
mkdir -p includes/KHR
mkdir -p shaders
mkdir -p assets

# Create placeholder files for GLAD (User needs to replace these with actual GLAD files if not using a package manager or if they want specific extensions)
# For now, we will assume the user might drop them in or we will try to compile without them if we can find them via CMake, 
# but the prompt asked for root/src/glad.c, so we create the file.
touch src/glad.c
touch includes/glad/glad.h
touch includes/KHR/khrplatform.h

# Create placeholder for stb_image
touch includes/stb_image.h

echo "Directory structure created."
