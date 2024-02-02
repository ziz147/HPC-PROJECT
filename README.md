# HPC-PROJECT

## Description du code
Le code parallèle est contenu dans le dossier **Code_NURBS_C**, et la version séquentielle est mis dans **Code_NURBS_sequentiel**.
Dans les deux dossiers, on retrouve:
- Un main **test_c** qui lit nos différentes matrices et qui fait les calculs, (vous devez decommenter les lignes saveCOOMatrix pour enregistrer les matrices de der_topo)

- Le fichier **Variables.dat** contient les paramètres à changer (DIM,length,length...). Vous pouvez aussi enregistrer les matrices de sortie en imposant 1 devant **write_BF_support**, **write_IND_mask_active** et **write_local**.

- Le fichier **matrix.c** n'existait pas dans le code python, mais il a été ajouté dans la version C du code pour faciliter les différentes opérations sur les matrices et les vecteurs.

- Les fichiers **hypersurface.c**, **surface.c** ,**local_Support_mM.c**, permettent de calculer la fonction local_support en 2d et en 3d.

- Le fichier **der_topo.c** regroupe la majorité des fonctions ajoutées pour la partie der_nurbs comme la sortie des liste le hashing pour enlever les parties dupliquées ou les différentes opérations algébriques sur nos matrices COO.

## Compilation et exécution du code
### Partie séquentielle
- Pour compiler le code, tapez sur le terminal la commande **make** et puis **./main** pour exécuter le programme généré.
- Pour nettoyer les fichiers objets, utilisez la commande **make clean** et **make test** pour supprimer les fichiers.txt.

### Partie Parallèle
- Cette partie a été paralléliser en OpenMP il faut alors avoir une version de gcc adapté à cette parallélisation.

- Pour compiler le code, tapez sur le terminal la commande **make** et puis **./main** pour exécuter le programme avec le maximum des processeurs disponibles ou avec **OMP_NUM_THREADS=N** **./main**

- Pour nettoyer les fichiers objets, utilisez la commande **make clean** et **make test** pour supprimer les fichiers.txt.

- Un script **./speedup.sh** a été ajouté pour enregistrer les temps d'exécution (il décommente les parties dans le main qui permettent d'afficher le temps d'exécution pour chaque fonction).

- Un notebook python a été ajouté pour tracer le speed_up et faire un post traitement des résultats parallèle.

## Validation des résultats
Pour cette partie, on a mis tout les codes utilisés dans le sous dossier 
**gradients/test_der_topo**

Pour refaire les tests, vous pouvez commencer par:

- Sélectionner une dimension et la partie nurbs ou b_spline (b_spline etait mis d'office sur local_support en python on modifie son entrée à 1 à la place de 0 en python et en c pour la tester).

- Lancer les calculs et enregistrer les matrices avec la partie C et modifier **problem_settings.py** pour commencer les tests avec notre notebook.

- Ouvrir le notebook et suivre les blocs qui permettent de tester toutes les matrices (Il ne faut pas faire la partie 2D de ter_topo pour l'instant avant de charger les matrices adaptés)

- Toutes les fonctions du python peuvent être compiler à partir du notebook,il faut juste avoir plus que 8GB de RAM disponible

 **On** **a** **ajouté** **des** **fonctions** **qui** **permettent** **de** **lire** **nos** **matrices** **COO** **dans** **ce** **notebook**, n'oubliez pas de supprimer les matrices une fois que vous avez testé le code pour éviter les problèmes de mémoire.


## Auteurs

- **NAIM Mouna**  - mnaim@bordeaux-inp.fr
- **REDOUANE Mohamed Youssef**   - mredouane@enseirb-matmeca.fr
- **EL GODE Amal**   - aelgode@bordeaux-inp.fr
- **Ziad Zahi**   - ziad.zahi@bordeaux-inp.fr
- **Willy Camille Touler Foka**   - wtoulerfoka@bordeaux-inp.fr


