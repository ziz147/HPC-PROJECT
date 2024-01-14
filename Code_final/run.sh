#!/bin/bash

# Exécuter la commande make
make

# Écrire "a" dans le terminal
echo "sortie de la fonction find span basis avec données test en C"

# Exécuter le premier exécutable
./x1

echo "sortie de la fonction find span basis avec données test en Python"
# Exécuter le script Python
python3 test_find_span_basis_py.py

echo "sortie de la fonction surface point avec données test en C "

# Exécuter le premier exécutable
./x2

echo "sortie de la fonction surface point avec données test en Python"
# Exécuter le script Python
python3 test_surface_point_py.py

make clean
