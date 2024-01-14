#include "derTopo.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare(const void *a, const void *b) {
    double val1 = *(const double *)a;
    double val2 = *(const double *)b;
    return (val1 > val2) - (val1 < val2);
}

void SortElements(const Vector *input, Vector *output) {
    // Tri du tableau d'entrée
    qsort(input->data, input->length, sizeof(double), compare);

    // Allouer de la mémoire pour output->data
    output->data = (double *)malloc(input->length * sizeof(double));
    if (output->data == NULL) {
        // Gérer l'erreur d'allocation
        exit(1);
    }

    // Copier les éléments uniques dans output->data
    int uniqueCount = 0;
    for (int i = 0; i < input->length; i++) {
        if (i == 0 || input->data[i] != input->data[i - 1]) {
            output->data[uniqueCount++] = input->data[i];
        }
    }

    // Réallouer output->data pour correspondre à la taille réelle
    output->data = (double *)realloc(output->data, uniqueCount * sizeof(double));
    if (output->data == NULL && uniqueCount > 0) {
        // Gérer l'erreur de réallocation
        exit(1);
    }

    // Ajuster la longueur du tableau de sortie
    output->length = uniqueCount;
}



// pour selectionner les lignes
void selectRows(const Matrix *source, const Vector *indices, Matrix *destination) {
    // Allouer de la mémoire pour destination
    destination->rows = indices->length;
    destination->cols = source->cols;
    destination->data = (double *)malloc(destination->rows * destination->cols * sizeof(double));

    if (destination->data == NULL) {
        // Gérer l'erreur d'allocation
        exit(1);
    }

    // Copier les lignes
    for (int i = 0; i < indices->length; i++) {
        int rowIndex = (int)indices->data[i]; // Assurez-vous que les indices sont des entiers
        for (int j = 0; j < source->cols; j++) {
            destination->data[i * destination->cols + j] = source->data[rowIndex * source->cols + j];
        }
    }
}

// pour selectionner les colonnes
void selectColumns(const Matrix *source, const Vector *columnIndices, Matrix *destination) {
    // Initialiser la matrice de destination
    destination->rows = source->rows;
    destination->cols = columnIndices->length;
    destination->data = (double *)malloc(destination->rows * destination->cols * sizeof(double));

    if (destination->data == NULL) {
        // Gérer l'erreur d'allocation
        exit(1);
    }

    // Copier les colonnes sélectionnées
    for (int i = 0; i < source->rows; i++) {
        for (int j = 0; j < columnIndices->length; j++) {
            int colIndex = (int)columnIndices->data[j]; // Assurez-vous que les indices sont des entiers
            destination->data[i * destination->cols + j] = source->data[i * source->cols + colIndex];
        }
    }
}



////////////
void extractElementsAndReshape(const Matrix *W, const Matrix *indices, Vector *output) {
    output->length = indices->rows;
    output->data = (double *)malloc(output->length * sizeof(double));

    if (output->data == NULL) {
        // Gérer l'erreur d'allocation
        exit(1);
    }

    for (int i = 0; i < indices->rows; i++) {
        int rowIndex = (int)indices->data[i * indices->cols];     // Première colonne d'IND_mask
        int colIndex = (int)indices->data[i * indices->cols + 1]; // Deuxième colonne d'IND_mask
        output->data[i] = W->data[rowIndex * W->cols + colIndex];
    }
}


//////


void matrixVectorMultiplication(const Matrix *A, const Vector *B, Vector *C) {
    // Assurez-vous que les dimensions sont compatibles pour la multiplication
    if (A->cols != B->length || C->length != A->rows) {
        printf("Erreur : dimensions incompatibles pour la multiplication matrice-vecteur\n");
        exit(1);
    }

    // Effectuez la multiplication matrice-vecteur
    for (int i = 0; i < A->rows; i++) {
        C->data[i] = 0;
        for (int j = 0; j < A->cols; j++) {
            C->data[i] += A->data[i * A->cols + j] * B->data[j];
        }
    }
}




void der_NURBS(ListOfVectors local_support , Matrix BF_support , Vector IND_mask_active , Matrix IND_mask, Matrix IND_mask_tot ,Matrix rho_e, Matrix P_rho , Matrix W, int DIM, Vector* der_CP, Vector* der_W, Vector* BF_mask){
    
    if (DIM == 2){

        /* local_support_flat */
        //traduction de :
        ///local_support_flat = [item for sublist in local_support for item in sublist]
        int local_support_SIZE  = local_support.size ,index = 0, tot_SIZE=0;
        Vector local_support_flat; 
        // Calcul de la taille du vecteur applatit 
        for (int i = 0; i < local_support.size; i++) {
            tot_SIZE += local_support.vectors[i].length;
        }   

        local_support_flat.length = tot_SIZE;
        local_support_flat.data = (double*) malloc(tot_SIZE * sizeof(double)); 
        // Vérification de l'allocation réussie
        if (local_support_flat.data == NULL) {
            // Gestion de l'erreur d'allocation
            exit(1);
        }
        // Applatir local_support en un vecteur 
        for (int i = 0 ; i < local_support_SIZE; i++){
            for( int j = 0 ; j < local_support.vectors[i].length ; j++ ){
            
            local_support_flat.data[index] = local_support.vectors[i].data[j];
            index +=1;
            }
        }
    //////////////
    //BF_mask = list(set(local_support_flat))
    /// ici set enleve les doublon et list les trie donc en C
     // On ajout <stdlib.h> pour la fonction qsort et on a crée la fonction compare pour les sorter en ordre croissant
     // Puis sorted element supprime les doublons (voir la fonction sortelements en haut)
        /* BF_mask */ 

        SortElements(&local_support_flat, &BF_mask);

        /* BF_support_temp */
        // BF_support_temp = BF_support[BF_mask,:] on a créer la fonction selectRows
        
        Matrix BF_support_rows_selected;
        selectRows(&BF_support, &BF_mask, &BF_support_rows_selected);
        Matrix BF_support_temp;
        selectColumns(&BF_support_rows_selected, &IND_mask_active, &BF_support_temp);
        free(BF_support_rows_selected.data);
        // W_temp = W[IND_mask[:,0],IND_mask[:,1]].reshape((len(IND_mask),1))
        // Utilisation pour W_temp
        Vector W_temp;
        extractElementsAndReshape(&W, &IND_mask, &W_temp);

        // Utilisation pour W_S_temp
        Vector W_S_temp;
        extractElementsAndReshape(&W, &IND_mask_tot, &W_S_temp);

        // Utilisation pour P_temp
        Vector P_temp;
        extractElementsAndReshape(&P_rho, &IND_mask, &P_temp);

        // Nij_w = BF_support_temp * W_temp.
        Matrix Nij_w;
        Nij_w.rows = BF_support_temp.rows;
        Nij_w.cols = BF_support_temp.cols;

        // Allouer de la mémoire pour Nij_w
        Nij_w.data = (double *)malloc(Nij_w.rows * Nij_w.cols * sizeof(double));
        hadamard_vector_product(&BF_support_temp, &W_temp, &Nij_w); // dans les fonctions de matrice

        //S_w = (BF_support.dot(W_S_temp))[BF_mask]


        Vector S_w_temp,S_w;
        S_w_temp.length=BF_support.rows;
        S_w_temp.data== (double *)malloc(S_w_temp.length * sizeof(double));
        matrixVectorMultiplication(&BF_support, &W_S_temp, &S_w_temp);
        S_w.length=BF_mask->length;
        S_w.data== (double *)malloc(S_w.length * sizeof(double));
        for (int i = 0; i < BF_mask->length; i++) {
        S_w.data[i] = S_w.data[(int)BF_mask->data[i]];
        }
        // der_CP = Nij_w/S_w
        Matrix der_CP;
        der_CP.rows=Nij_w.rows;
        der_CP.cols=Nij_w.cols;
        der_CP.data = (double *)malloc(Nij_w.rows * Nij_w.cols * sizeof(double));
        for (int i = 0; i < Nij_w.rows; i++) {
            for (int j = 0; j < Nij_w.cols; j++) {
                der_CP.data[i * der_CP.cols + j] = Nij_w.data[i * Nij_w.cols + j] / S_w.data[i];
            }
        }


        //Nij_P = (BF_support_temp * P_temp.T)
        Matrix Nij_P;
        Nij_P.rows = BF_support_temp.rows;
        Nij_P.cols = BF_support_temp.cols;
        Nij_P.data = (double *)malloc(Nij_P.rows * Nij_P.cols * sizeof(double));
        hadamard_vector_product(&BF_support_temp, &P_temp, &Nij_P);

        //manque encore les lignes Nij_nurbs = (BF_support_temp * rho_e[BF_mask])
        // et der_W = Nij_P/S_w - Nij_nurbs/S_w





    } else { // DIM =3

        // la 3d sera pareil avec quelques modification a tester pour demain
    }
}




Matrix der_BSPLINE(Vector IND_mask_active, Matrix BF_support) {
    // Example implementation
    // Allocate memory for the result matrix
    // Translate Python logic into C
    // Return the result
}