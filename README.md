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

## Come Avviare
Basta eseguire lo script:
```bash
./run.sh
```
Usa **W, A, S, D** per muoverti.
