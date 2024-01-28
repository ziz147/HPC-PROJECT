#include "derTopo.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



unsigned int hash(double value) {
    // Basic hash function for demonstration
    return (unsigned int)(value * 1000) % HASH_TABLE_SIZE;
}

Node* createNode(double data) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        exit(1);  // Handle memory allocation failure
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}
void deleteNode(HashTable *table, double data) {
    unsigned int index = hash(data);
    Node *current = table->buckets[index];
    Node *previous = NULL;

    while (current != NULL) {
        if (current->data == data) {
            if (previous == NULL) {
                // Le nœud à supprimer est le premier de la liste
                table->buckets[index] = current->next;
            } else {
                // Le nœud à supprimer n'est pas le premier
                previous->next = current->next;
            }
            free(current);
            break;
        }
        previous = current;
        current = current->next;
    }
}
void insertHashTable(HashTable *table, double data) {
    unsigned int index = hash(data);
    Node *newNode = createNode(data);

    // Insert at the beginning of the list to handle collision
    newNode->next = table->buckets[index];
    table->buckets[index] = newNode;
}

int existsInHashTable(HashTable *table, double data) {
    unsigned int index = hash(data);
    Node *temp = table->buckets[index];
    while (temp != NULL) {
        if (temp->data == data) {
            return 1;  // Found
        }
        temp = temp->next;
    }
    return 0;  // Not found
}

void freeHashTable(HashTable *table) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Node *temp = table->buckets[i];
        while (temp != NULL) {
            Node *toDelete = temp;
            temp = temp->next;
            free(toDelete);
        }
    }
}

int compare(const void *a, const void *b) {
    double val1 = *(const double *)a;
    double val2 = *(const double *)b;
    return (val1 > val2) - (val1 < val2);
}
/*
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
            output->data[uniqueCount] = input->data[i];
            uniqueCount+=1;
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
*/

void SortElements(const Vector *input, Vector *output) {
    HashTable table = {0};
    for (int i = 0; i < input->length; i++) {
        if (!existsInHashTable(&table, input->data[i])) {
            insertHashTable(&table, input->data[i]);
        }
    }
    // Allouer de la mémoire pour output->data
    output->data = (double *)malloc(input->length * sizeof(double));
    if (output->data == NULL) {
        // Gérer l'erreur d'allocation
        freeHashTable(&table);
        exit(1);
    }

     int uniqueCount = 0;
    for (int i = 0; i < input->length; i++) {
    if (existsInHashTable(&table, input->data[i])) {
        output->data[uniqueCount++] = input->data[i];
        // Supprimer de la table de hachage pour éviter les doublons
        deleteNode(&table, input->data[i]);
    }
}

    // Adjust the length of the output array
    output->length = uniqueCount;

    // Clean up
    freeHashTable(&table);
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




void selectRowsCOO(COOMatrix *source, const Vector *indices, COOMatrix *destination) {
    // Compter le nombre d'éléments non nuls dans les lignes sélectionnées
    int nonZeroCount = source->nonZeroCount;
    // Allouer de la mémoire pour destination
    Matrix A;
    A=convertToDense(source);
    destination->values = (double *)malloc(nonZeroCount * sizeof(double));
    destination->rowsIndices = (int *)malloc(nonZeroCount * sizeof(int));
    destination->colsIndices = (int *)malloc(nonZeroCount * sizeof(int));
    int idx = 0;
    for (int i = 0; i < indices->length; i++) {
        int rowIndex = (int)indices->data[i]; // Assurez-vous que les indices sont des entiers
        for (int j = 0; j < source->cols; j++) {
            if (A.data[rowIndex * source->cols + j] !=0)
            {
                destination->values[idx] = A.data[rowIndex * source->cols + j];
                destination->rowsIndices[idx] = source->rowsIndices[i];
                destination->colsIndices[idx] = source->colsIndices[i];
                idx++;
            }
        }
    }
    
    free(A.data);
    destination->values = (double *)realloc(destination->values, idx * sizeof(double));
    destination->rowsIndices = (int *)realloc(destination->rowsIndices, idx * sizeof(int));
    destination->colsIndices = (int *)realloc(destination->colsIndices, idx * sizeof(int));
    //destination->depthsIndices = (int *)malloc(nonZeroCount * sizeof(int));
    destination->nonZeroCount = idx;
    destination->rows = indices->length;
    destination->cols = source->cols;
    destination->depth = source->depth;

}





void selectColumnsCOO(COOMatrix *source, const Vector *columnIndices, COOMatrix *destination) {
    // Compter le nombre d'éléments non nuls dans les colonnes sélectionnées
    int nonZeroCount = source->nonZeroCount;
    // Allouer de la mémoire pour destination
    Matrix A;
    A=convertToDense(source);
    destination->values = (double *)malloc(nonZeroCount * sizeof(double));
    destination->rowsIndices = (int *)malloc(nonZeroCount * sizeof(int));
    destination->colsIndices = (int *)malloc(nonZeroCount * sizeof(int));
    int idx = 0;

    for (int i = 0; i < columnIndices->length; i++) {
        for (int j = 0; j < source->cols; j++) {
            int colIndex = (int)columnIndices->data[j];
            if (A.data[i * source->cols + colIndex ] !=0)
            {
                destination->values[idx] = A.data[i * source->cols + colIndex];
                destination->rowsIndices[idx] = source->rowsIndices[i];
                destination->colsIndices[idx] = source->colsIndices[i];
                idx++;
            }
        }
    }
    
    free(A.data);
    destination->values = (double *)realloc(destination->values, idx * sizeof(double));
    destination->rowsIndices = (int *)realloc(destination->rowsIndices, idx * sizeof(int));
    destination->colsIndices = (int *)realloc(destination->colsIndices, idx * sizeof(int));
    //destination->depthsIndices = (int *)malloc(nonZeroCount * sizeof(int));
    destination->nonZeroCount = idx;
    destination->rows = source->rows;
    destination->cols = columnIndices->length;
    destination->depth = source->depth;
}








////////////
void extractElementsAndReshape(const Matrix *W, const Matrix *indices, Vector *output) {
    output->length = indices->rows;
    output->data = (double *)malloc(output->length * sizeof(double));

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
    for (int i = 0; i < A->nonZeroCount; i++) {
        int row = A->rowsIndices[i];
        int col = A->colsIndices[i];
        double value = A->values[i];

        // Ajouter la contribution de cet élément au produit matrice-vecteur
        C->data[row] += value * B->data[col];
    }
    
}




void der_NURBS(ListOfVectors local_support , COOMatrix BF_support , Vector IND_mask_active , Matrix IND_mask, Matrix IND_mask_tot ,Vector rho_e, Matrix P_rho , Matrix W, int DIM, Matrix* der_CP, COOMatrix *der_W, Vector* BF_mask){
    
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
            fprintf(stderr, " ERREUR ALLOCATION \n");
            exit(1);
        }
        // Applatir local_support en un vecteur 
        for (int i = 0 ; i < local_support_SIZE; i++){
            for( int j = 0 ; j < local_support.vectors[i].length ; j++ ){
            
            local_support_flat.data[index] = local_support.vectors[i].data[j];
            index +=1;
            }
        }
/*

local_support_flat.data[index]=local_support.vectors[0].data[0];
        index+=1;
        for (int i = 0 ; i < local_support_SIZE; i++){
            for( int j = 0 ; j < local_support.vectors[i].length ; j++ ){
                int test=0;
            for (int k = 0; k < index; k++)
            {

                
                if(local_support_flat.data[k]==local_support.vectors[i].data[j])
                {
                    test=1;
                    break;
                }
                
            }
            if(test==0){
                local_support_flat.data[index] = local_support.vectors[i].data[j];
            index +=1;
            }
            
            }
        }
*/
    //////////////
    //BF_mask = list(set(local_support_flat))
    /// ici set enleve les doublon et list les trie donc en C
     // On ajout <stdlib.h> pour la fonction qsort et on a crée la fonction compare pour les sorter en ordre croissant
     // Puis sorted element supprime les doublons (voir la fonction sortelements en haut)
        /* BF_mask */ 
        fprintf(stderr, " Check Point \n");
        SortElements(&local_support_flat, BF_mask);
        saveVector(*BF_mask, "testooo.txt");
        fprintf(stderr, " Check Point ok %d \n", local_support_flat.length);
       // saveVector(*BF_mask, "BF_mask_c.txt");
        free(local_support_flat.data); 
        local_support_flat.data=NULL;
        
        /* BF_support_temp */
        // BF_support_temp = BF_support[BF_mask,:] on a créer la fonction selectRows
        
        COOMatrix BF_support_rows_selected;

        //selectRows(&BF_support, BF_mask, &BF_support_rows_selected);
        fprintf(stderr, " Check Point \n");
        selectRowsCOO(&BF_support, BF_mask, &BF_support_rows_selected);
        fprintf(stderr, " Check Point \n");
        COOMatrix BF_support_temp;
        selectColumnsCOO(&BF_support_rows_selected, &IND_mask_active, &BF_support_temp);
        freeCOOMatrix(&BF_support_rows_selected);
        fprintf(stderr, " Check Point \n");
        //selectColumns(&BF_support_rows_selected, &IND_mask_active, &BF_support_temp);
       // free(BF_support_rows_selected.data);
        // W_temp = W[IND_mask[:,0],IND_mask[:,1]].reshape((len(IND_mask),1))
        // Utilisation pour W_temp
        Vector W_temp;
        fprintf(stderr, " Check Point \n");
        extractElementsAndReshape(&W, &IND_mask, &W_temp);
        // Utilisation pour W_S_temp
        

        
        fprintf(stderr, " Check Point \n");
        // Nij_w = BF_support_temp * W_temp.
        
        COOMatrix Nij_w;
        // Initialiser Nij_w en copiant la structure de BF_support_temp
        copyCOOMatrixStructure(&BF_support_temp, &Nij_w);

        // Effectuer le produit d'Hadamard
        hadamardVectorProductCOO(&BF_support_temp, &W_temp, &Nij_w);
        fprintf(stderr, " Check Point \n");
        free(W_temp.data); 
        W_temp.data=NULL;
        //S_w = (BF_support.dot(W_S_temp))[BF_mask]


        Vector S_w_temp,S_w;
        S_w_temp.length=BF_support.rows;
        
        S_w_temp.data= (double *)malloc(S_w_temp.length * sizeof(double));

        fprintf(stderr, " Check Point 2 \n");
        Vector W_S_temp;
        extractElementsAndReshape(&W, &IND_mask_tot, &W_S_temp);
        matrixVectorMultiplicationCOO(&BF_support, &W_S_temp, &S_w_temp);
        free(W_S_temp.data);
        W_S_temp.data=NULL;
        fprintf(stderr, " Check Point 3 \n");
     //   matrixVectorMultiplication(&BF_support, &W_S_temp, &S_w_temp);
        
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
        fprintf(stderr, " Check Point 4 \n");
        free(S_w_temp.data);
        S_w_temp.data=NULL;
        // der_CP = Nij_w/S_w
        
        der_CP->rows=Nij_w.rows;
        der_CP->cols=Nij_w.cols;
        der_CP->data = (double *)malloc(Nij_w.rows * Nij_w.cols * sizeof(double));

        // a coriger
        
        for (int i = 0; i < Nij_w.rows * Nij_w.cols; i++) {
            der_CP->data[i] = 0.0;
        }
        
        for (int i = 0; i < Nij_w.nonZeroCount; i++) {
            int row = Nij_w.rowsIndices[i];
            int col = Nij_w.colsIndices[i];
            double value = Nij_w.values[i];

            der_CP->data[row * der_CP->cols + col] = value / S_w.data[row];
        }
        //
        freeCOOMatrix(&Nij_w); /// a crér une fonction freeCOO
                                                               
        //Nij_P = (BF_support_temp * P_temp.T)
        COOMatrix Nij_P;
        copyCOOMatrixStructure(&BF_support_temp, &Nij_P);
        fprintf(stderr, " Check Point 5 \n");
        // Utilisation pour P_temp
        Vector P_temp;
        extractElementsAndReshape(&P_rho, &IND_mask, &P_temp);
        hadamardVectorProductCOO(&BF_support_temp, &P_temp, &Nij_P);
        free(P_temp.data);
        P_temp.data=NULL;
        fprintf(stderr, " Check Point \n");
        




        //Nij_nurbs = (BF_support_temp * rho_e[BF_mask])
        /*

        Vector rho_e_masked;
        // ici a verifier
        rho_e_masked.length = BF_support_temp.cols;

        rho_e_masked.data = (double *)malloc(rho_e_masked.length * sizeof(double));
        // Extraction des éléments de rho_e correspondants à BF_mask
        for (int i = 0; i < BF_mask->length; i++) {
            rho_e_masked.data[i] = rho_e.data[(int)BF_mask->data[i]];
        }
        free(rho_e_masked.data);
        rho_e_masked.data=NULL;

        */
        COOMatrix Nij_nurbs;
        copyCOOMatrixStructure(&BF_support_temp, &Nij_nurbs);
        //fprintf(stderr,"compar: %d et %d \n",BF_support_temp.cols,rho_e_masked.length );
        hadamardVectorProductCOO2(&BF_support_temp, &rho_e, &Nij_nurbs,*BF_mask);
        
        fprintf(stderr, " Check Point 1 \n");
        free(BF_support_temp.values);
        BF_support_temp.values =NULL;
        fprintf(stderr, " Check Point 1 \n");
        


        
        //free(rho_e_masked.data);
        // der_W = Nij_P/S_w - Nij_nurbs/S_w
        
        der_W->cols= Nij_P.cols;
        der_W->rows= Nij_P.rows;
        der_W->depth= Nij_P.depth;
        int maxind=(int)fmin(Nij_P.nonZeroCount,Nij_nurbs.nonZeroCount);
        fprintf(stderr,"maax: %d  \n",maxind );
      //  der_W->rowsIndices= (int *)malloc( maxind* sizeof(int));
        fprintf(stderr, " Check Point 1 \n");
      //  der_W->colsIndices= (int *)calloc( maxind, sizeof(int));
        fprintf(stderr, " Check Point 2 \n");
        //der_W->depthsIndices= (int *)malloc( maxind* sizeof(int));
     //   der_W->values= (double *)calloc( maxind, sizeof(double));
        fprintf(stderr, " Check Point 3 \n");
       /* Matrix passage;
       passage.rows=der_W->rows;
        passage.cols=der_W->cols;
        passage.data=(double *)malloc( passage.cols*passage.rows* sizeof(double));
        
        for (int i = 0; i <passage.cols*passage.rows; i++) {
            passage.data[i]=0.0;
            
        }
        for (int i = 0; i < Nij_P.nonZeroCount; i++) {
            int row = Nij_P.rowsIndices[i];
            int col = Nij_P.colsIndices[i];
            double value = Nij_P.values[i];

            passage.data[row * passage.cols + col] += value / S_w.data[row];
        }
       for (int i = 0; i < Nij_nurbs.nonZeroCount; i++) {
            int row = Nij_nurbs.rowsIndices[i];
            int col = Nij_nurbs.colsIndices[i];
            double value = Nij_nurbs.values[i];

            passage.data[row * passage.cols + col] -= value / S_w.data[row];
        }
        int idx = 0;
        for (int i = 0; i < passage.rows; i++) {
            for (int j = 0; j < passage.cols; j++) {
                
                    double val =matrice(passage, i, j);
                    if (val != 0) {
                        der_W->values[idx] = val;
                        der_W->rowsIndices[idx] = i;
                        der_W->colsIndices[idx] = j;
                        der_W->depthsIndices[idx] = 0;
                        idx++;
                    
                }
            }
        }
        
*/
    for (int i = 0; i < maxind; i++) {
            int row = Nij_nurbs.rowsIndices[i];
            int col = Nij_nurbs.colsIndices[i];
            double value =(Nij_P.values[i]-Nij_nurbs.values[i])/ S_w.data[row];
            der_W->values[i] = value;
            der_W->rowsIndices[i] = row;
            der_W->colsIndices[i] = col;
           // der_W->depthsIndices[i] = 0;
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