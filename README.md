# HPC-PROJECT

## Description du code
Le code final est contenu dans le dossier Code_final.  
- Le fichier **Variables.dat** contient les paramètres à changer (DIM,length,length...). Vous pouvez aussi enregistrer les matrices de sortie en imposant 1 devant write_BF_support, write_IND_mask_active et write_local.

- Le fichier **matrix.c** n'existait pas dans le code python, mais il a été ajouté dans la version C du code pour faciliter les différentes opérations sur les matrices et les vecteurs.

## Compilation et exécution du code
- Pour compiler le code, tapez sur le terminal la commande **make** et puis **./main** pour exécuter le programme généré.
- Pour nettoyer les fichiers objets, utilisez la commande **make clean** et **make test** pour supprimer les fichiers.txt.
- Un fichier **main.py** a été ajouté pour enregistrer les matrices de sorties.
  



