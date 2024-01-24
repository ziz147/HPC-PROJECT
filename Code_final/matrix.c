/* On définit ici les différentes fonctions  qui ont été
 utilisées dans les différentes fonctions du code*/
 //Ces fonctions fonctions permettent de gérer les opérations effectuées
 //sur les matrices et les vecteurs


#include "matrix.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//Charger la matrice à partir d'un fichier 
Matrix loadMatrix(const char *filename) {
    Matrix result;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    int rows = 0;
    int cols = 0;
    char ch;
    while(!feof(file)) {
        ch = fgetc(file);
        if(ch == '\n'){
            rows++;
        }
        if(ch == ' ' && rows == 0){
            cols++;
        }
    }
    cols++; // Incrementer cols car le nombre de colonnes est le nombre d'espaces + 1

    rewind(file);

    // Charger le nombre de lignes et de colonnes dans la matrice
    result.rows = rows;
    result.cols = cols;

    // Allouer la mémoire pour la matrice
    result.data = (double *)malloc(result.rows * result.cols * sizeof(double));

    // Charger les valeurs de la matrice
    /*
    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            if (fscanf(file, "%lf", &result.data[i * result.cols + j]) != 1) {
                fprintf(stderr, "Erreur lors de la lecture de la valeur (%d, %d) de la matrice\n", i + 1, j + 1);
                exit(EXIT_FAILURE);
            }
            // Ignorer les espaces entre les valeurs
            while (fgetc(file) == ' ');
        }
    }
    */
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            fscanf(file, "%lf", &result.data[i * result.cols + j]);
        }
    }

    fclose(file);
    return result;
}

// Charger une matrice qui a un format COO
COOMatrix loadCOO(const char *filename) {
    COOMatrix coo;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Lire le nombre d'éléments non nuls
    fscanf(file, "%d", &coo.nnz);

    // Allouer de la mémoire pour les tableaux COO
    coo.rows = (int *)malloc(coo.nnz * sizeof(int));
    coo.cols = (int *)malloc(coo.nnz * sizeof(int));
    coo.data = (double *)malloc(coo.nnz * sizeof(double));

    // Lire les éléments de la matrice COO depuis le fichier
    for (int k = 0; k < coo.nnz; k++) {
        fscanf(file, "%d %d %lf", &coo.rows[k], &coo.cols[k], &coo.data[k]);
    }

    fclose(file);

    return coo;
}

//Charger un vecteur à partir d'un fichier(U1.dat par exemple) 
Vector loadVector(const char *filename) {
    Vector result;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Compter le nombre de lignes dans le fichier
    int numLines = 0;
    char c;
    while (fscanf(file, "%c", &c) != EOF) {
        if (c == '\n') {
            numLines++;
        }
    }

    // Réinitialiser la position du curseur au début du fichier
    rewind(file);

    // Charger la longueur du vecteur
    result.length = numLines;
    result.data = (double *)malloc(result.length * sizeof(double));

    // Charger les valeurs du vecteur
    for (int i = 0; i < result.length; i++) {
        if (fscanf(file, "%lf", &result.data[i]) != 1) {
            fprintf(stderr, "Erreur lors de la lecture de la valeur %d du vecteur\n", i + 1);
            exit(EXIT_FAILURE);
        }
    }

    fclose(file);
    return result;
}

//Afficher le contenu de la matrice 
void printMatrix(const Matrix matrix) {
    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.cols; j++) {
            printf("%lf", matrix.data[i * matrix.cols + j]);
            if (j < matrix.cols - 1) {
                printf(" ");
            }
        }
        printf("\n");
    }
}

//Afficher le contenu du vecteur
void printVector(Vector vector) {
    for (int i = 0; i < vector.length; i++) {
        printf("%lf ", vector.data[i]);
    }
    printf("\n");
}

//Extraire une colonne spécifique de la  matrice sous forme de vecteur.
Vector col(const Matrix matrix, int i) {
    if (i < 0 || i >= matrix.cols) {
        fprintf(stderr, "Error: Invalid column index\n");
        exit(EXIT_FAILURE);
    }

    Vector result;
    result.length = matrix.rows;
    result.data = (double *)malloc(result.length * sizeof(double));

    for (int row = 0; row < matrix.rows; row++) {
        result.data[row] = matrice(matrix, row, i);
    }

    return result;
}

//Extraire une ligne spécifique de la  matrice sous forme de vecteur.
Vector line(const Matrix matrix, int i) {
    if (i < 0 || i >= matrix.rows) {
        fprintf(stderr, "Error: Invalid column index\n");
        exit(EXIT_FAILURE);
    }

    Vector result;
    result.length = matrix.cols;
    result.data = (double *)malloc(result.length * sizeof(double));

    for (int cols = 0; cols < matrix.cols; cols++) {
        result.data[cols] = matrice(matrix, i, cols);
    }

    return result;
}

//Extraire une colonne spécifique d'une matrice 3D sous forme de matrice 2D
Matrix col3d(const Matrix matrix, int j) {
    if (j < 0 || j >= matrix.cols) {
        fprintf(stderr, "Error: Invalid column index\n");
        exit(EXIT_FAILURE);
    }

    Matrix result;
    int row = matrix.rows;
    initmatrice3d(result, row, 1, 1);

    for (int i = 0; i < row; i++) {
        result.data[i] = matrice(matrix, i, j);
    }

    return result;
}

void hadamard_vector_product(const Matrix *P, const Vector *w, Matrix *p_w) {
    // Vérifier si les dimensions sont compatibles
    if (P->rows != w->length || P->rows != p_w->rows || P->cols != p_w->cols) {
        // Gérer l'erreur ou retourner, selon vos besoins
        return;
    }

    // Effectuer la multiplication Hadamard
    for (int i = 0; i < P->rows; i++) {
        for (int j = 0; j < P->cols; j++) {

            matrice(*p_w, i, j) = matrice(*P, i, j) * w->data[j];
        }
    }

    // Définir les dimensions de p_w
    p_w->rows = P->rows;
    p_w->cols = P->cols;
}

//Calculer le produit Hadamard de deux matrices
//(Ce produit a été utilisé dans la fonction Hypersurface.c)
void hadamard_product(const Matrix *P, const Matrix *w, Matrix *p_w) {
    // Vérifier si les dimensions sont compatibles
    if (P->rows != w->rows || P->cols != w->cols || P->rows != p_w->rows || P->cols != p_w->cols) {
        // Gérer l'erreur ou retourner, selon vos besoins
        return;
    }

    // Effectuer la multiplication Hadamard
    for (int i = 0; i < P->rows; i++) {
        for (int j = 0; j < P->cols; j++) {

            matrice(*p_w, i, j) = matrice(*P, i, j) * matrice(*w, i, j);
        }
    }

    // Définir les dimensions de p_w
    p_w->rows = P->rows;
    p_w->cols = P->cols;
}

//Calcule le produit Hadamard de deux matrices 3D
void hadamard_product3d(Matrix P, Matrix w, Matrix *p_w) {
    // Vérifier si les dimensions sont compatibles
    if (P.rows != w.rows || P.cols != w.cols || P.depth != w.depth ||
        P.rows != p_w->rows || P.cols != p_w->cols || P.depth != p_w->depth) {
        // Gérer l'erreur ou retourner, selon vos besoins
        return;
    }

    // Effectuer la multiplication Hadamard
    for (int i = 0; i < P.rows; i++) {
        for (int j = 0; j < P.cols; j++) {
            for (int k = 0; k < P.depth; k++) {
                matrice3d(*p_w, i, j, k) = matrice3d(P, i, j, k) * matrice3d(w, i, j, k);
            }
        }
    }

    // Définir les dimensions de p_w
    p_w->rows = P.rows;
    p_w->cols = P.cols;
    p_w->depth = P.depth;
}

//Intialiser la taille d'une matrice 
void definem(Matrix* M, int i, int j) {
    M->rows = i;
    M->cols = j;
    M->data = (double*)malloc(i * j * sizeof(double));
}

//Initialiser la matrice telle que M(i,j) = i+j+2
void initializeMatrix(Matrix *mat,int n,int m) {
    mat->rows = n;
    mat->cols = m;
    mat->data = (double*)malloc(n * m * sizeof(double));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matricep(mat,i,j) = i+j+2;  // Assigning random values between 0 and 1
        }
    }
}

//Initialiser un vecteur tel que v(i) = i+1
void initializeVecteur(Vector *vec,int n) {
    vec->length = n;
    vec->data = (double*)malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) {
            vec->data[i] = i+1;  // Assigning random values between 0 and 1
        }
    }

//Initialiser la matrice avec des zéros
void mzero(Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            matricep(mat,i,j)=0.;
        }
    }
}

//Sauvegarder un vecteur dans un fichier
void saveVector(Vector vector, const char *filename) {
    FILE *file = fopen(filename, "w");

    if (file != NULL) {
        for (int i = 0; i < vector.length; i++) {
            fprintf(file, "%lf ", vector.data[i]);
            fprintf(file, "\n");
        }
        

        fclose(file);
    } else {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", filename);
    }
}

//Sauvegarder la matrice dans un fichier
void saveMatrix(Matrix matrix, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.cols; j++) {
            fprintf(file, "%lf", matrice(matrix, i, j));
            if (j < matrix.cols - 1) {
                fprintf(file, " ");
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

//Sauvegarder la matrice en format COO dans un fichier 
void saveCOO(COOMatrix coo, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    // Écrire le nombre d'éléments non nuls
    fprintf(file, "%d\n", coo.nnz);

    // Écrire les éléments de la matrice COO
    for (int i = 0; i < coo.nnz; i++) {
        fprintf(file, "%d %d %lf\n", coo.rows[i], coo.cols[i], coo.data[i]);
    }

    fclose(file);
}

//Libérer la mémoire allouée pour un vecteur
void freeVector(Vector *vector) {
    free(vector->data);
    vector->length = 0;
}




//Ajouter un vecteur à une liste de vecteurs
void addVectorToList(ListOfVectors *listOfVectors, int vectorLength) {
    // Utiliser realloc pour agrandir le tableau de vecteurs
    listOfVectors->vectors = (Vector *)realloc(listOfVectors->vectors, (listOfVectors->size + 1) * sizeof(Vector));

    // Initialiser le nouveau vecteur ajouté
    initializeVecteur(&listOfVectors->vectors[listOfVectors->size], vectorLength);

    // Mettre à jour le nombre total de vecteurs dans la liste
    listOfVectors->size++;
}



//Extraire les valeurs d'une colonne spécifique d'une matrice
Vector extractValues(Matrix BF_Support, int* ind, int k, int size) {
    Vector extractedValues;
    extractedValues.data = (double*)malloc(size * sizeof(double));
    for (int i = 0; i < size; i++) {
        extractedValues.data[i] = matrice(BF_Support, ind[i],k);
    }
    return extractedValues;
}



//Sauvegarder une liste de vecteurs dans un fichier
void saveListOfVectors( ListOfVectors listOfVectors, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    for (int i = 0; i < listOfVectors.size; i++) {
        for (int j = 0; j < listOfVectors.vectors[i].length; j++) {
            fprintf(file, "%d", (int)listOfVectors.vectors[i].data[j]);
            if (j < listOfVectors.vectors[i].length - 1) {
                fprintf(file, " ");
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
}


//Libérer une liste de vecteurs
void freeListOfVectors(ListOfVectors *list) {
    if (list != NULL) {
        // Libérer chaque vecteur dans la liste
        for (int i = 0; i < list->size; i++) {
            if (list->vectors[i].data != NULL) {
                free(list->vectors[i].data);
            }
        }

        // Libérer le tableau de vecteurs
        free(list->vectors);

        // Réinitialiser la taille de la liste
        list->size = 0;
    }
}

Vector Mat_vec_product(Matrix A, Vector B) {
    Vector result;
    result.length = A.rows;

    result.data = (double *)malloc(result.length * sizeof(double));

    if (result.data == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < result.length; i++) {
        result.data[i] = 0.0;
        for (int j = 0; j < A.cols; j++) {
            result.data[i] += matrice(A, i, j) * B.data[j];
        }
    }

    return result;
}

COOMatrix denseToCOO(Matrix matrix) {
    COOMatrix coo;
    int rows = matrix.rows;
    int cols = matrix.cols;
    coo.nnz = 0;

    // Compter le nombre d'éléments non nuls
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (fabs(matrice(matrix, i, j)) > 1e-10) {
                coo.nnz++;
            }
        }
    }

    // Allouer de la mémoire pour les tableaux COO
    coo.rows = (int *)malloc(coo.nnz * sizeof(int));
    coo.cols = (int *)malloc(coo.nnz * sizeof(int));
    coo.data = (double *)malloc(coo.nnz * sizeof(double));

    int nnz_index = 0;

    // Remplir les tableaux COO
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (fabs(matrice(matrix, i, j)) > 1e-10) {
                coo.rows[nnz_index] = i;
                coo.cols[nnz_index] = j;
                coo.data[nnz_index] = matrice(matrix, i, j);
                nnz_index++;
            }
        }
    }

    return coo;
}


Matrix COOtoDense(COOMatrix coo, int rows, int cols) {
    // Allouer la matrice dense
    Matrix denseMatrix ;
    denseMatrix.rows = rows;
    denseMatrix.cols = cols;
    denseMatrix.data = (double*)malloc(rows * cols * sizeof(double));

    // Initialiser la matrice dense à zéro
    for (int i = 0; i < rows * cols; i++) {
        denseMatrix.data[i] = 0.0;
    }

    // Remplir la matrice dense à partir de la matrice COO
    for (int k = 0; k < coo.nnz; k++) {
        matrice(denseMatrix,coo.rows[k],coo.cols[k]) = coo.data[k];
    }

    return denseMatrix;
}

// Remplir la matrice avec des 1
    void m_ones(Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            matricep(mat, i, j) = 1.0;
        }
    }
}
