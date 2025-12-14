# SysAdmin Game - Progetto OpenGL

Questo progetto è un semplice gioco 3D sviluppato in C++ e OpenGL, dove interpreti un tecnico informatico (SysAdmin) in una server room.

## Caratteristiche Principali

### 1. Sistema di Build (CMake)
Abbiamo configurato il progetto utilizzando **CMake** per gestire la compilazione in modo automatico e multipiattaforma.
-   È incluso uno script `run.sh` che configura, compila ed esegue il gioco con un solo comando.

### 2. Ambiente di Gioco
-   **Pavimento**: Un piano 20x20 texturizzato.
-   **Muri**: 4 pareti perimetrali che delimitano l'area di gioco.
-   **Telecamera**: Una visuale isometrica/dall'alto che segue il giocatore per dare una buona visione della stanza.

### 3. Il Personaggio (SysAdmin)
Il giocatore non è un semplice cubo, ma un **modello gerarchico** composto da parti separate:
-   Testa, Corpo, Braccia (Destro/Sinistro), Gambe (Destra/Sinistra).
-   **Animazione**: Abbiamo implementato una camminata realistica usando funzioni trigonometriche (`sin`) per far oscillare braccia e gambe a tempo mentre ci si muove.

### 4. I Server (PC)
-   **Generazione**: I server non sono posizionati a mano, ma generati **proceduralmente** in una griglia 4x4.
-   **Collisioni**: Il giocatore non può attraversare i server o uscire dalla mappa. Abbiamo implementato un sistema di collisioni AABB (Axis-Aligned Bounding Box) che permette anche di "scivolare" lungo gli ostacoli.

### 5. Texturing e Grafica
-   **Pixel Art**: Abbiamo creato texture personalizzate in stile "pixel art" per il personaggio (faccia, vestiti da tecnico, jeans).
-   **Texture Separate**: Ogni parte del corpo ha la sua texture specifica (es. la testa ha la faccia solo davanti e i capelli dietro).
-   **Filtro**: Usiamo il filtro `GL_NEAREST` per mantenere la grafica nitida e "pixelosa" senza sfocature.

## Prerequisiti e Installazione

Prima di avviare il gioco, assicurati di aver installato le dipendenze necessarie.

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
È necessario avere **CMake** e un compilatore C++ (es. Visual Studio o MinGW). GLFW viene gestito tramite CMake, ma potrebbe essere necessario scaricare i binari precompilati se non presenti nel path.

## Come Avviare
Basta eseguire lo script:
```bash
./run.sh
```
Usa **W, A, S, D** per muoverti.

## Project Report & Features

This section indicates how the delivered application satisfies the requested features for the exam.

### 1. Topic Adherence
*The interactive application must adhere to the topic of the groups' project delivered at the end of the course.*
- **Compliance**: The application **"SysAdmin Game"** is strictly related to the group project theme. It simulates a server room environment where the player (a SysAdmin) moves around to maintain servers.
- **Reusability**: The assets (server textures, character style) and the core logic (grid movement, camera) are designed to be reused and expanded for the final group project.

### 2. Playable Application
*Implementing a playable application with minimal logic.*
- **gameplay**: The user can launch the game, navigate a 3D environment, and interact with the system via a GUI menu (Start/Exit).
- **Logic**: Simple collision detection prevents walking through objects, and game states (Menu vs Game) dictate the interaction mode.

### 3. Input Handling
*Handling of input events, i.e., mouse and keyboards events.*
- **Keyboard**:
    - **Movement**: `W`, `A`, `S`, `D` control the character's position.
    - **Actions**: `Space` is mapped for interaction (repairing servers).
    - **System**: `ESC` allows quitting or returning to menu.
- **Mouse**:
    - **Menu Interaction**: The ImGui integration relies on mouse coordinates and clicks to interact with buttons.
    - **Camera Control**: While currently fixed for isometric gameplay, the architecture supports mouse look integration.

### 4. Geometric Transformations & Animation
*Using geometric transformations to animate contents (e.g., moving objects or camera).*
- **Hierarchical Animation**: The character uses a hierarchical model (Body -> Arms/Legs) where limbs oscillate using `sin(time)` functions to simulate walking.
- **Camera**: The view matrix transforms the scene to follow the player dynamically.
- **World**: Translation and Scaling matrices are used to position and size the procedural server grid and map boundaries.

### 5. Simple Objects & Textures
*Complex objects are not required, simple placeholders with textures are acceptable.*
- **Models**: We use cubes (primitives) to construct complex looking objects (Servers, Character) by combining them.
- **Texturing**: All objects are fully textured using custom Pixel Art assets loaded via `stb_image`.

