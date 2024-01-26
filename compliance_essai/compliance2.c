#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "matrix.h"
#include "compliance2.h"

//Tout comme Mouna ,ces variables sont contenus dans Problem_Setting.py.
double *c_vec; //Variable globale
char* ANSYS_comline_opt;
char* del_string;
char* con_fun;
char* flag_BCs;
char* WD;
double DERIVATES ; 

int compterLignesDansFichier(const char *nomFichier) {
    FILE *fid;  // Déclare un pointeur vers un objet fichier
    fid = fopen(nomFichier, "r");  // Ouvre le fichier en mode lecture

    if (fid == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return -1;  // Retourne -1 en cas d'erreur
    }

    int ligne_count = 0;
    char caractere;

    // Parcourt le fichier caractère par caractère
    while ((caractere = fgetc(fid)) != EOF) {
        if (caractere == '\n') {
            ligne_count++;  // Incrémente le compteur lorsque vous trouvez un saut de ligne ('\n')
        }
    }

    fclose(fid);  // Ferme le fichier

    return ligne_count;  // Retourne le nombre de lignes comptées
}

void rearrangeStrainM( Vector U, Vector strain_M) {
    
    int size = U.length;

    
    Vector *indices, *strain_M_copy ; // Allocation dynamique de mémoire pour indices
    initVector(indices ,size);
    initVector(strain_M_copy, size);
    if (indices == NULL) {
        printf("Erreur d'allocation de mémoire.\n");
        return;
    }

    if(U.length != strain_M.length)
    {
        printf("pas la meme taille de vecteur , impossible");
    }

    // Remplir le tableau indices à partir de la première colonne de ELEMENTS_macro
    for (int i = 0; i < size; i++) {
        indices->data[i] = (int)U.data[i];
        strain_M_copy->data[i] =  strain_M.data[i];
    }

    // Soustraire 1 à chaque élément de indices pour obtenir des indices 0-based
    for (int i = 0; i < size; i++) {
        indices->data[i] -= 1;
    }

    // Mettre à jour le tableau strain_M en utilisant les indices calculés
    for (int i = 0; i < size; i++) {
        int j = (int)indices->data[i];
        strain_M.data[i] = strain_M_copy->data[j];
    }

    free(indices);
    freeVector(indices);
    freeVector(strain_M_copy);
}


// Vector compliance_grad_fun(Vector rho_e, Matrix P_rho, Matrix W, Matrix ELEMENTS, Matrix IND_mask, ListOfVectors local_support, Matrix BF_suport, Vector IND_mask_tot, Vector IND_mask_active, Vector *arg_vector, Matrix *arg_Matrix , ListOfVectors *arg_ListOfVector, const char* flag_scale,char** op_dir_parts, int num_parts)
// {

//     Vector rho_e_M,IND_mask_M_tot, IND_mask_active_M ,ELEMENTS_micro; 
//     Matrix P_rho_M, W_M, ELEMENTS_macro, IND_mask_M, BF_support_M;
//     ListOfVectors local_support_M ;

//     FILE *fid;
    

//     if ( DERIVATES == 1)
//     {
//         if (strcmp(flag_scale, "micro_macro") == 0)
//         {
//             rho_e_M = arg_vector [0] ;
//             IND_mask_M_tot = arg_vector [1] ;
//             IND_mask_active_M = arg_vector [2] ;

//             P_rho_M = arg_Matrix[0];
//             W_M = arg_Matrix[1] ;
//             ELEMENTS_macro = arg_Matrix[2] ;
//             IND_mask_M = arg_Matrix[3] ;
//             BF_suport = arg_Matrix[4] ;

//             local_support_M = arg_ListOfVector[0] ;


//         } 
//         else if (strcmp(flag_scale, "micro") == 0)
//         {
//             ELEMENTS_micro= arg_vector[0] ;
//             ELEMENTS_macro[0]=ELEMENTS_micro;
//         }

//         /*Reading files*/
//             // Calculez la longueur totale nécessaire pour la chaîne finale
//             int total_length = 0;
//             for (int i = 0; i < num_parts; ++i) {
//             total_length += strlen(op_dir_parts[i]);
//             }

//             // Allouez de la mémoire pour la chaîne finale
//             char op_dir[total_length + 1];  // +1 pour le caractère nul de fin de chaîne
//             op_dir[0] = '\0';  // Initialisez avec une chaîne vide

//             // Concaténez toutes les chaînes
//             for (int i = 0; i < num_parts; ++i) 
//             {
//                 strcat(op_dir, op_dir_parts[i]);
//             }

//             // Changez le répertoire de travail
//             if (chdir(op_dir) != 0) {
//                 perror("chdir failed");
//             }
        
//         if (strcmp(flag_scale, "micro_macro") == 0 || strcmp(flag_scale, "micro"))
//         {
//             /*Reading micro strain energy*/
//                 fid = fopen("FEM_SIMP_RES_micro.txt", "r");
//                 if (fid == NULL) 
//                 {
//                     perror("Erreur lors de l'ouverture du fichier");
//                     return 1 ; 
//                     // Quitte le programme avec une erreur 
//                 }

//                 const char *FEM_SIMP_RES_micro = "FEM_SIMP_RES_micro.txt";
//                 c_vec_micro = 2*loadVector(FEM_SIMP_RES_micro);

//                 fclose(fid);

//             /*Reading macro strain energy*/

//             fid=open("STRAIN_ELEMENTS.txt", "r");
//             if (fid == NULL) 
//             {
//                 perror("Erreur lors de l'ouverture du fichier");
//                 return 1 ; 
//                 // Quitte le programme avec une erreur 
//             }

//             const char *STRAIN_ELEMENTS = "STRAIN_ELEMENTS.txt";
//             strain_M = 2*loadVector(STRAIN_ELEMENTS);



//             rearrangeStrainM(line(ELEMENTS_macro, 0), Vector strain_M);



            
        
//         }

//     }

   





// }