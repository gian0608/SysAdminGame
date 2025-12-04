#!/bin/bash
# Configura il progetto (crea la cartella build se non esiste)
cmake -S . -B build

# Compila il progetto
cmake --build build

# Se la compilazione ha successo, avvia il gioco
if [ $? -eq 0 ]; then
    ./build/SysAdminGame
else
    echo "Compilazione fallita."
fi
