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




void selectRowsCOO(const COOMatrix *source, const Vector *indices, COOMatrix *destination) {
    // Compter le nombre d'éléments non nuls dans les lignes sélectionnées
    int nonZeroCount = 0;
    for (int i = 0; i < source->nonZeroCount; i++) {
        for (int j = 0; j < indices->length; j++) {
            if (source->rowsIndices[i] == (int)indices->data[j]) {
                nonZeroCount++;
                break;
            }
        }
    }

    // Allouer de la mémoire pour destination
    destination->values = (double *)malloc(nonZeroCount * sizeof(double));
    destination->rowsIndices = (int *)malloc(nonZeroCount * sizeof(int));
    destination->colsIndices = (int *)malloc(nonZeroCount * sizeof(int));
    destination->depthsIndices = (int *)malloc(nonZeroCount * sizeof(int));
    destination->nonZeroCount = nonZeroCount;
    destination->rows = indices->length;
    destination->cols = source->cols;
    destination->depth = source->depth;

    if (destination->values == NULL || destination->rowsIndices == NULL || destination->colsIndices == NULL || destination->depthsIndices == NULL) {
        // Gérer l'erreur d'allocation
        exit(1);
    }

    // Copier les éléments non nuls des lignes sélectionnées
    int idx = 0;
    for (int i = 0; i < source->nonZeroCount; i++) {
        for (int j = 0; j < indices->length; j++) {
            if (source->rowsIndices[i] == (int)indices->data[j]) {
                destination->values[idx] = source->values[i];
                destination->rowsIndices[idx] = source->rowsIndices[i];
                destination->colsIndices[idx] = source->colsIndices[i];
            destination->depthsIndices[idx] = source->depthsIndices[i];
            idx++;
            break;
        }
    }
}
}





void selectColumnsCOO(const COOMatrix *source, const Vector *columnIndices, COOMatrix *destination) {
    // Compter le nombre d'éléments non nuls dans les colonnes sélectionnées
    int nonZeroCount = 0;
    for (int i = 0; i < source->nonZeroCount; i++) {
        for (int j = 0; j < columnIndices->length; j++) {
            if (source->colsIndices[i] == (int)columnIndices->data[j]) {
                nonZeroCount++;
                break;
            }
        }
    }

    // Allouer de la mémoire pour destination
    destination->values = (double *)malloc(nonZeroCount * sizeof(double));
    destination->rowsIndices = (int *)malloc(nonZeroCount * sizeof(int));
    destination->colsIndices = (int *)malloc(nonZeroCount * sizeof(int));
    destination->depthsIndices = (int *)malloc(nonZeroCount * sizeof(int));
    destination->nonZeroCount = nonZeroCount;
    destination->rows = source->rows;
    destination->cols = columnIndices->length;
    destination->depth = source->depth;

    if (destination->values == NULL || destination->rowsIndices == NULL || destination->colsIndices == NULL || destination->depthsIndices == NULL) {
        // Gérer l'erreur d'allocation
        exit(1);
    }

    // Copier les éléments non nuls des colonnes sélectionnées
    int idx = 0;
    for (int i = 0; i < source->nonZeroCount; i++) {
        for (int j = 0; j < columnIndices->length; j++) {
            if (source->colsIndices[i] == (int)columnIndices->data[j]) {
                destination->values[idx] = source->values[i];
                destination->rowsIndices[idx] = source->rowsIndices[i];
                destination->colsIndices[idx] = source->colsIndices[i];
                destination->depthsIndices[idx] = source->depthsIndices[i];
                idx++;
                break;
            }
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
        int rowIndex = (int)matricep(indices,i,0);    // Première colonne d'IND_mask
        int colIndex = (int)matricep(indices,i,1); // Deuxième colonne d'IND_mask
        
        output->data[i] = matricep(W, rowIndex, colIndex);
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
void matrixVectorMultiplicationCOO(const COOMatrix *A, const Vector *B, Vector *C) {
    // Initialiser le vecteur C à 0
    for (int i = 0; i < C->length; i++) {
        C->data[i] = 0.0;
        
    }

    // Effectuer la multiplication matrice-vecteur pour les éléments non nuls
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->cols; j++) {
            double value = GET_COO_VALUE(*A, i, j); // Supposant que depthIdx est toujours 0 pour une matrice 2D
            C->data[i] += value * B->data[j];
        }
    }
}




void der_NURBS(ListOfVectors local_support , COOMatrix BF_support , Vector IND_mask_active , Matrix IND_mask, Matrix IND_mask_tot ,Vector rho_e, Matrix P_rho , Matrix W, int DIM, Matrix* der_CP, COOMatrix* der_W, Vector* BF_mask){
    
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

        SortElements(&local_support_flat, BF_mask);
        free(local_support_flat.data); 

        /* BF_support_temp */
        // BF_support_temp = BF_support[BF_mask,:] on a créer la fonction selectRows
        
        COOMatrix BF_support_rows_selected;

        //selectRows(&BF_support, BF_mask, &BF_support_rows_selected);
        selectRowsCOO(&BF_support, BF_mask, &BF_support_rows_selected);

        COOMatrix BF_support_temp;
        selectColumnsCOO(&BF_support_rows_selected, &IND_mask_active, &BF_support_temp);

        //selectColumns(&BF_support_rows_selected, &IND_mask_active, &BF_support_temp);
       // free(BF_support_rows_selected.data);
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
        
        COOMatrix Nij_w;
        // Initialiser Nij_w en copiant la structure de BF_support_temp
        copyCOOMatrixStructure(&BF_support_temp, &Nij_w);

        // Effectuer le produit d'Hadamard
        hadamardVectorProductCOO(&BF_support_temp, &W_temp, &Nij_w);
        fprintf(stderr, " Check Point \n");
        free(W_temp.data); 
        //S_w = (BF_support.dot(W_S_temp))[BF_mask]


        Vector S_w_temp,S_w;
        S_w_temp.length=BF_support.rows;
        
        S_w_temp.data= (double *)malloc(BF_support.rows * sizeof(double));
        if (local_support_flat.data == NULL) {
            fprintf(stderr, " ERREUR ALLOCATION \n");
            exit(1);
        }
        
        matrixVectorMultiplicationCOO(&BF_support, &W_S_temp, &S_w_temp);
     //   matrixVectorMultiplication(&BF_support, &W_S_temp, &S_w_temp);
        free(W_S_temp.data); 
        S_w.length=BF_mask->length;
        S_w.data= (double *)malloc(S_w.length * sizeof(double));
        for (int i = 0; i < BF_mask->length; i++) {
            int index = (int)BF_mask->data[i]; // Obtenir l'indice de BF_mask
            S_w.data[i] = S_w_temp.data[index];
            // Vérifier que l'indice est valide
            if (index < 0 || index >= S_w_temp.length) {
            printf("Erreur : indice en dehors des limites dans S_w\n");
            exit(1);
            }

            
        }
        free(S_w_temp.data);
        // der_CP = Nij_w/S_w
        fprintf(stderr, " Check Point \n");
        der_CP->rows=Nij_w.rows;
        der_CP->cols=Nij_w.cols;
        der_CP->data = (double *)malloc(Nij_w.rows * Nij_w.cols * sizeof(double));
        
        
        for (int i = 0; i < Nij_w.rows; i++) {
            for (int j = 0; j < Nij_w.cols; j++) {
                der_CP->data[i * der_CP->cols + j] = GET_COO_VALUE(Nij_w, i, j) / S_w.data[i];
            }
        }
        free(Nij_w.values); /// a crér une fonction freeCOO
                                                               
        //Nij_P = (BF_support_temp * P_temp.T)
        COOMatrix Nij_P;
        copyCOOMatrixStructure(&BF_support_temp, &Nij_P);
        
        hadamardVectorProductCOO(&BF_support_temp, &P_temp, &Nij_P);
        free(P_temp.data);
        fprintf(stderr, " Check Point \n");
        




        //Nij_nurbs = (BF_support_temp * rho_e[BF_mask])
        COOMatrix Nij_nurbs;
        copyCOOMatrixStructure(&BF_support_temp, &Nij_nurbs);

        Vector rho_e_masked;
        // ici a verifier
        rho_e_masked.length = BF_support_temp.cols;

        rho_e_masked.data = (double *)malloc(rho_e_masked.length * sizeof(double));
        // Extraction des éléments de rho_e correspondants à BF_mask
        for (int i = 0; i < BF_mask->length; i++) {
            rho_e_masked.data[i] = rho_e.data[(int)BF_mask->data[i]];
        }
        fprintf(stderr,"compar: %d et %d \n",BF_support_temp.cols,rho_e_masked.length );
        hadamardVectorProductCOO(&BF_support_temp, &rho_e_masked, &Nij_nurbs);
        
        //free(rho_e_masked.data);
        // der_W = Nij_P/S_w - Nij_nurbs/S_w

        der_W->cols= Nij_P.cols;
        der_W->rows= Nij_P.rows;
        der_W->depth= Nij_P.depth;
        int maxind=(Nij_P.nonZeroCount+Nij_nurbs.nonZeroCount);

        der_W->rowsIndices= (int *)malloc( maxind* sizeof(int));

        der_W->colsIndices= (int *)malloc( maxind* sizeof(int));

        //der_W->depthsIndices= (int *)malloc( maxind* sizeof(int));
        der_W->values= (double *)malloc( maxind* sizeof(double));
    
        int count = 0;  // Un compteur pour les éléments ajoutés à der_W

        for (int i = 0; i < der_W->rows; i++) {
            
            for (int j = 0; j < der_W->cols; j++) {
                
            double result =0;
            result=(GET_COO_VALUE(Nij_P,i,j) - GET_COO_VALUE(Nij_nurbs,i,j));

            // Ajouter le résultat à der_W si non nul
            if (result != 0) {
                // Assurez-vous qu'il y a suffisamment d'espace dans der_W
                
                der_W->rowsIndices[count] = i;
                der_W->colsIndices[count] = j;
                der_W->values[count] = result/ S_w.data[i];

                count++;

            }
            
        }
        }


        fprintf(stderr, "OUUUF\n");

                   
                
                           
     //   free(S_w.data);               

                     
       // free(Nij_P.values);             
        //free(Nij_nurbs.values);

    } else { // DIM =3

        // la 3d sera pareil avec quelques modification a tester 
    }
}




Matrix der_BSPLINE(Vector IND_mask_active, Matrix BF_support) {
    // Example implementation
    // Allocate memory for the result matrix
    // Translate Python logic into C
    // Return the result
}