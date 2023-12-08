#include "local_Support_mM.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

/* On définit ici les fonctions dans l'ordre suivant: ls_2d_numba, ls_3d_numba,
 local_support_fun (Dans le code python cette fonction a été définie en premier)*/


//Cette fonction calcul le support local en 2D

void ls_2d_numba(Matrix IND_mask_tot, Matrix IND_mask, Matrix u1, Matrix u2, Vector U1, Vector U2, int p1_temp, int p2_temp, int n1_temp, int n2_temp, ListOfVectors *local_Support, Matrix *BF_Support, Vector *IND_mask_active) {

    // Obtenir la taille de la matrice IND_mask
    int N_ROWS_mask = IND_mask.rows;
    int N_COLS_mask = IND_mask.cols;

    // Copie de la matrice IND_mask_temp
    Matrix IND_mask_temp;
    definem(&IND_mask_temp, N_ROWS_mask, N_COLS_mask);
    for (int i = 0; i < N_ROWS_mask; i++) {
        for (int j = 0; j < N_COLS_mask; j++) {
            matricep(&IND_mask_temp, i, j) = matrice(IND_mask, i, j);
        }
    }

    // Allouer de l'espace pour BF_Support
    definem(BF_Support, u1.rows, IND_mask_tot.rows);
    mzero(BF_Support);

    // Initialiser les listes IND_mask_active et local_Support
    IND_mask_active->length = 0;
    IND_mask_active->data = (double*)malloc(1 * sizeof(double));
    local_Support->vectors = (Vector *)malloc(1 * sizeof(Vector));
    local_Support->size = 0;

    // Pour chaque CP du modèle (actif et inactif)
    for (int k = 0; k < IND_mask_tot.rows; k++) {

        // Obtenir les indices liés à IND_mask_active
        int IND_mask_tot_temp_cols = IND_mask_tot.cols;
        int IND_mask_temp_cols = IND_mask_temp.cols;
        int IND_mask_temp_rows = IND_mask_temp.rows;

        int *IND_mask_tot_temp = malloc(IND_mask_tot_temp_cols * sizeof(int));
        for (int i = 0; i < IND_mask_tot_temp_cols; i++) {
            IND_mask_tot_temp[i] = matrice(IND_mask_tot, k, i);
        }

        // Vérification de l'appartenance de IND_mask_tot_temp à IND_mask_temp 
        int found = 1;
        for (int i = 0; i < IND_mask_temp_rows; i++) {
            found = 1;
            for (int j = 0; j < IND_mask_temp_cols; j++) {
                if (matricep(&IND_mask_temp, i, j) != IND_mask_tot_temp[j]) {
                    found = 0;
                    break;
                }
            }
            if (found == 1) {
                break;
            }
        }

        free(IND_mask_tot_temp);



        int *ind_temp = malloc(u1.rows * sizeof(int));
        int count = 0;

        //Parcours des points de contrôle 
        for (int i = 0; i < u1.rows; i++) {
            double u1_val = u1.data[i];
            double u2_val = u2.data[i];

            // Vérifier les conditions 
            //la deuxieme condition a été enlevé, ie pour le ou et on a ajouté <= pour fusionner les deux conditions
            int b1 = (u1_val <= U1.data[(int)matrice(IND_mask_tot, k, 0) + p1_temp + 1] && u1_val >= U1.data[(int)matrice(IND_mask_tot, k, 0)]);
            int b2 = (u2_val <= U2.data[(int)matrice(IND_mask_tot, k, 1) + p2_temp + 1] && u2_val >= U2.data[(int)matrice(IND_mask_tot, k, 1)]);
            if (b1 && b2) {
                ind_temp[count] = i;
                count++;
            }
        }

        //Allocation et copie des indices i
        int *ind = malloc(count * sizeof(int));
        for (int i = 0; i < count; i++)
        {
            ind[i]=ind_temp[i];
        }
        free(ind_temp);



        //Si l'indice se trouve dans IND_Mask_temp
        if (found == 1) {

            // Ajouter l'indice à IND_mask_active
            // Réallouer la mémoire pour IND_mask_active
            int newSize = IND_mask_active->length + 1;
            IND_mask_active->data = (double *)realloc(IND_mask_active->data, newSize * sizeof(double));
            IND_mask_active->length =newSize;
            IND_mask_active->data[newSize-1] = k;

///////////// crée une nouvelle list et alloue ind avec count
            // Ajouter les indices à local_Support
            
            // Mettre à jour local_Support
            addVectorToList(local_Support, count);
            for (int i = 0; i < count; i++) {
                local_Support->vectors[local_Support->size-1].data[i] = ind[i];
            }

            // Libérer la mémoire allouée
            
        }
            // Évaluer le produit de la fonction de base des éléments appartenant au support local 
            //(essayer sur tous les éléments appartenant à LS)
            Matrix P_rho_aux;
            definem(&P_rho_aux, n1_temp + 1, n2_temp + 1);
            mzero(&P_rho_aux);
            matrice(P_rho_aux, (int)matrice(IND_mask_tot, k, 0), (int)matrice(IND_mask_tot, k, 1)) = 1.0;
            Matrix u;
            Matrix v;
            Matrix w;
            definem(&u, count, 1);
            definem(&v, count, 1);
            definem(&w, n1_temp + 1, n2_temp + 1);
            for (int i = 0; i < count; i++) {
                int idx = ind[i];
                
                
                u.data[i] = u1.data[idx];
                
                
                v.data[i] = u2.data[idx];
                
            }

            // Initialisation de w avec des valeurs à 1
            for (int i1 = 0; i1 < n1_temp+1; i1++)
            {
                for (int i2 = 0; i2 < n2_temp; i2++)
                {
                    matrice(w,i1,i2)=1;
                }
                
            }
            Vector tempo;

            // Appel de la fonction SurfacePoint_fun_numba
            tempo = SurfacePoint_fun_numba(u, n1_temp, p1_temp, U1, v, n2_temp, p2_temp, U2, P_rho_aux, w, 0);

                // Libérer la mémoire allouée
                
            for (int i = 0; i < count; i++) {
                matricep(BF_Support, ind[i],k) = tempo.data[i];
            }
            free(ind);
            free(tempo.data);
            free(u.data);
            free(v.data);
            free(w.data);
            // Libérer la mémoire allouée
            free(P_rho_aux.data);
        }
    }




 //Calcul du local support en 3D

 void ls_3d(Matrix IND_mask_tot, Matrix IND_mask, Matrix u1, Matrix u2,Matrix u3 , Vector U1, 
    Vector U2, Vector U3, int p1_temp, int p2_temp, int p3_temp, int n1_temp, int n2_temp, int n3_temp,
    ListOfVectors *local_Support, Matrix *BF_Support, Vector *IND_mask_active){
    
    int N_ROWS_mask = IND_mask.rows;
    int N_COLS_mask = IND_mask.cols;

    // Copie de la matrice IND_mask_temp
    Matrix  IND_mask_temp;
    definem(&IND_mask_temp, N_ROWS_mask, N_COLS_mask);
    for (int i = 0; i < N_ROWS_mask; i++) {
        for (int j = 0; j < N_COLS_mask; j++) {
            matricep(&IND_mask_temp, i, j) = matrice(IND_mask, i, j);
        }
    }

    //Allocation de l'espace pour BF_Support
    definem(BF_Support, u1.rows, IND_mask_tot.rows);
    mzero(BF_Support);
    IND_mask_active->length = 0;
    IND_mask_active->data = (double*)malloc(1 * sizeof(double));
    local_Support->vectors = (Vector *)malloc(1 * sizeof(Vector));
    local_Support->size = 0;


    //Pour chaque CP du modèele (actif et inactif)
    for (int k = 0; k < IND_mask_tot.rows; k++) {
        int IND_mask_tot_temp_cols = IND_mask_tot.cols;
        int IND_mask_temp_cols = IND_mask_temp.cols; // ?
        int IND_mask_temp_rows = IND_mask_temp.rows; // ?

        int *IND_mask_tot_temp = malloc(IND_mask_tot_temp_cols * sizeof(int));
        for (int i = 0; i < IND_mask_tot_temp_cols; i++) {
            IND_mask_tot_temp[i] = matrice(IND_mask_tot, k, i);
        }

        int found = 1;
        for (int i = 0; i < IND_mask_temp_rows; i++) {
            found = 1;
            for (int j = 0; j < IND_mask_temp_cols; j++) {
                if (matricep(&IND_mask_temp, i, j) != IND_mask_tot_temp[j]) {
                    found = 0;
                    break;
                }
            }
            if (found == 1) {
                break;
            }
        }

        free(IND_mask_tot_temp);
        //int listes=0;


        int *ind_temp = malloc(u1.rows * sizeof(int));
        int count = 0;

        //Parcours des points de contrôle 
        for (int i = 0; i < u1.rows; i++) {
            double u1_val = u1.data[i];
            double u2_val = u2.data[i];
            double u3_val = u3.data[i];

            // Vérifier les conditions 
            //La deuxieme condition a été enlevé == pour le ou et on a ajouté <= pour fusionner les deux conditions ???
            int b1 = (u1_val <= U1.data[(int)matrice(IND_mask_tot, k, 0) + p1_temp + 1] && u1_val >= U1.data[(int)matrice(IND_mask_tot, k, 0)]);
            int b2 = (u2_val <= U2.data[(int)matrice(IND_mask_tot, k, 1) + p2_temp + 1] && u2_val >= U2.data[(int)matrice(IND_mask_tot, k, 1)]);
            int b3 = (u3_val <= U3.data[(int)matrice(IND_mask_tot, k, 2) + p3_temp + 1] && u3_val >= U3.data[(int)matrice(IND_mask_tot, k, 2)]);
            
            if (b1 && b2 && b3) {
                    ind_temp[count] = i;
                    count++; 
                
            }
        }

        //Allocation et copie des indices
        int *ind = malloc(count * sizeof(int));
        for (int i = 0; i < count; i++)
        {
            ind[i]=ind_temp[i];
        }
        free(ind_temp);


        //Si l'indice est trouvé dans IND_mask_temp
        if (found == 1) {
            int newSize = IND_mask_active->length + 1;
            IND_mask_active->data = (double *)realloc(IND_mask_active->data, newSize * sizeof(double));
            IND_mask_active->length =newSize;
            IND_mask_active->data[newSize-1] = k;

            //Ajout des indices à local_Support 
            addVectorToList(local_Support, count);
        for (int i = 0; i < count; i++) {

  //  printf("Iteration %d: local_Support->size = %d, ind[%d] = %d\n", i, local_Support->size, i, ind[i]);
             local_Support->vectors[(local_Support->size - 1)].data[i] = ind[i];
            }
            
            
        }

        Matrix P_rho_aux;
        initmatrice3d(P_rho_aux, n1_temp + 1, n2_temp + 1, n3_temp +1);
        matrice3d(P_rho_aux, (int)matrice(IND_mask_tot, k, 0), (int)matrice(IND_mask_tot, k, 1),(int)matrice(IND_mask_tot, k, 2)) = 1.0;
        Matrix u;
        Matrix v;
        Matrix w;
        Matrix w_t;
        initmatrice3d(u, count, 1,1);
        initmatrice3d(v, count, 1,1);
        initmatrice3d(w , count , 1,1);
        initmatrice3dones(w_t, n1_temp + 1, n2_temp + 1, n3_temp + 1);
        for (int i = 0; i < count; i++) {
                int idx = ind[i];
                
                
                u.data[i] = u1.data[idx];
                v.data[i] = u2.data[idx];
                w.data[i] = u3.data[idx];
                
            }

        Matrix tempo;
        //Appel à la fonction BF_Support
        tempo = HyperSurfacePoint_fun( n1_temp, p1_temp, U1, n2_temp, p2_temp, U2,n3_temp, p3_temp, U3,  P_rho_aux, w_t, u, v, w, 0);
                
        for (int i = 0; i < count; i++) {
            matricep(BF_Support, ind[i],k) = tempo.data[i];
        }
        //Libérer la mémoire
        free(tempo.data);
        free(u.data);
        free(v.data);
        free(w.data);
        free(ind);
        free(P_rho_aux.data);

    }

}



//La fonction principale pour calculer le support local en fonction de la dimension
void local_support_fun(int p1, int p2, int p3, int n1, int n2, int n3, int DIM, Matrix ELEMENTS, Matrix IND_mask, Matrix IND_mask_tot, Vector U1, Vector U2, Vector U3, ListOfVectors *local_Support, Matrix *BF_Support, Vector *IND_mask_active) {
        int p1_temp = p1;
        int p2_temp = p2;
        int p3_temp = p3;
        int n1_temp = n1;
        int n2_temp = n2;
        int n3_temp = n3;
        Matrix u1,u2,u3;
        if (DIM==2)
        {
            u1= col3d(ELEMENTS,14);
            u2= col3d(ELEMENTS,15);
            ls_2d_numba(IND_mask_tot, IND_mask, u1, u2, U1, U2, p1_temp, p2_temp, n1_temp, n2_temp, local_Support, BF_Support, IND_mask_active);

        }
        if (DIM==3)
        {
            u1= col3d(ELEMENTS,14);
            u2= col3d(ELEMENTS,15);
            u3= col3d(ELEMENTS,16);
            ls_3d(IND_mask_tot, IND_mask, u1, u2, u3, U1, U2, U3, p1_temp, p2_temp, p3_temp, n1_temp, n2_temp, n3_temp,local_Support, BF_Support, IND_mask_active);
        }

        
       
}












