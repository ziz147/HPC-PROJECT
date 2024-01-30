#!/bin/bash

# Fichier de sortie pour les temps d'exécution
OUTPUT_FILE="execution_times.txt"

# Nettoyer le fichier de sortie existant
> $OUTPUT_FILE

# Boucle sur un ensemble de valeurs pour OMP_NUM_THREADS
for num_threads in {1..8}; do
    export OMP_NUM_THREADS=$num_threads
    output=$(./main)
    
    # Extraire le temps d'exécution du programme
    parallel_time=$(echo "$output" | grep "Temps d'exécution" | awk '{print $4}')

    # Enregistrer le nombre de threads et le temps d'exécution
    echo "$parallel_time" >> $OUTPUT_FILE
done

echo "Les temps d'exécution ont été enregistrés dans $OUTPUT_FILE"
