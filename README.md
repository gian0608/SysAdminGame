# SysAdmin Game Instructions

## Prerequisites
To compile and run this project, you need to provide a few library files that could not be generated automatically.

### 1. GLAD (OpenGL Loader)
You need to generate the GLAD files for OpenGL 3.3 Core.
1. Go to [https://glad.dav1d.de/](https://glad.dav1d.de/)
2. Language: C/C++
3. API: gl -> Version 3.3
4. Profile: Core
5. Click "Generate"
6. Download the zip.
7. Copy `src/glad.c` to `src/glad.c` in this project (replace the empty file).
8. Copy `include/glad/glad.h` to `includes/glad/glad.h` (replace the empty file).
9. Copy `include/KHR/khrplatform.h` to `includes/KHR/khrplatform.h` (replace the empty file).

### 2. stb_image (Texture Loading)
1. Download `stb_image.h` from [https://github.com/nothings/stb/blob/master/stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)
2. Place it in `includes/stb_image.h` (replace the empty file).

### 3. Textures
Place your texture images in the `assets/` folder.
- `assets/container.jpg` (Used for the Player and Servers)
- `assets/floor.jpg` (Used for the Floor)

You can use any jpg/png images.

## Building
Once the files are in place:
```bash
cmake .
make
./SysAdminGame
```

## Controls
- **WASD**: Move the Technician (Player).
- **SPACE**: Fix a broken server (Red) when close to it.
