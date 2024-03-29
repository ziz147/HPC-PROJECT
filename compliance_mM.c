#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "matrix.h"

//Ces variables sont contenus dans Problem_Setting.py. Je vais les adapter après
double *c_vec; //Variable globale
char* ANSYS_comline_opt;
char* del_string;
char* con_fun;
char* flag_BCs;
char* WD;

double compliance_c(Vector rho_e, Matrix ELEMENTS,const char* flag_scale, Vector args1, Matrix args2,char** op_dir_parts, int* c_index, int num_parts, double c)
{ 	
	Vector rho_e_M;
	Matrix ELEMENTS_macro;
	//Matrix mask;
	FILE *fid;
	//double* c = 0;
	double temp;
	
	if (strcmp(flag_scale, "micro_macro") == 0) 
	   {
		rho_e_M = args1;
		ELEMENTS_macro = args2;
		//printf("Fonction compliance_c: flag_scale est 'micro_macro'\n");
    	}
    	   
		
	    // Calculez la longueur totale nécessaire pour la chaîne finale
	    int total_length = 0;
	    for (int i = 0; i < num_parts; ++i) {
		total_length += strlen(op_dir_parts[i]);
	    }

	    // Allouez de la mémoire pour la chaîne finale
	    char op_dir[total_length + 1];  // +1 pour le caractère nul de fin de chaîne
	    op_dir[0] = '\0';  // Initialisez avec une chaîne vide

	    // Concaténez toutes les chaînes
	    for (int i = 0; i < num_parts; ++i) 
		{
			strcat(op_dir, op_dir_parts[i]);
	    }

	    // Changez le répertoire de travail
	    if (chdir(op_dir) != 0) {
			perror("chdir failed");
	    }


	    //printf("Le tableau contient %d éléments.\n", num_parts);
	
	    //Récupérer le nombre de ligne de ELEMENTS
	    int size_element, size_element_macro;
	    ELEMENTS.rows = size_element;
	    ELEMENTS_macro.rows = size_element_macro;
	    double** mask;
	    mask = (double **)malloc(size_element * sizeof(double *));
		if (mask == NULL) {

		// L'allocation pour mask a échoué, gérer l'erreur ici
			perror("Échec de l'allocation pour mask");
			exit(EXIT_FAILURE); // Ou toute autre gestion d'erreur appropriée
		}
	    for (int i = 0; i < size_element; i++) {
		 mask[i] = (double *)malloc(2 * sizeof(double));
		 if (mask[i] == NULL) {
        // L'allocation pour mask[i] a échoué, gérer l'erreur ici
        perror("Échec de l'allocation pour mask[i]");
            }
           	    
	    //mask.rows = size_element;
	    //mask.cols = 2;
	    
	    for (int i=0; i<size_element;i++)
	    {
	    	mask[i][0] = ELEMENTS.data[i];
	    	//printf("%f", mask[i][0]);
	    	mask[i][1] = rho_e.data[i];
	    }
	    
	    fid = fopen("rho_e.txt", "w");
	    if (fid == NULL) {
		perror("Erreur lors de l'ouverture du fichier");
	    }
	     
	    // Écrire les données dans le fichier
	    for (int i = 0; i < size_element; i++) {
		fprintf(fid, "%23.15e %23.15e\n", mask[i][0], mask[i][1]);
	    }
	    
	    // Fermer le fichier
    	fclose(fid);
    	    //free(fid);
    	    
    	if (strcmp(flag_scale, "micro_macro") == 0) 

	   	{
		double** mask_macro;
		mask_macro = (double **)malloc(size_element * sizeof(double *));
	    for (int i = 0; i < size_element; i++) 
		{
		mask_macro[i] = (double *)malloc(2 * sizeof(double));
        }
            	
            fid = fopen("rho_e_M.txt", "w");
            	
            for (int i = 0; i < size_element_macro; i++) {
		    	fprintf(fid, "%23.15e %23.15e\n", mask_macro[i][0], mask_macro[i][1]);
	    	}
	    	
	    	fclose(fid);
    	    	//free(fid);
            	
    	}
    	    
    	   int ANSYS_stat = 100;
    	   
    	   while (ANSYS_stat != 0 && ANSYS_stat != 1 && ANSYS_stat != 8) 
		   {
        	ANSYS_stat = system(ANSYS_comline_opt);
        	system(del_string);
    		}
    	    
			if (strstr(con_fun, "compliance") != NULL) 
			{
				//Vector c_index = args1; 
				if (*c_index == 0) {
					fid = fopen("FEM_SIMP_RES.txt", "r");
				} else if (*c_index == 1) {
					fid = fopen("FEM_SIMP_RES_mom.txt", "r");
				}
    		} 

			else 
			{
				fid = fopen("FEM_SIMP_RES.txt", "r");
				if (strcmp(flag_BCs, "mixed-w") == 0 || strcmp(flag_BCs, "mixed-c") == 0) 
				{
					
					while (fscanf(fid, "%lf", &temp) == 1) 
					{
        				c = temp;
					}

				}

				else {
					
					while (fscanf(fid, "%lf", &temp) == 1) {
						c_vec = &temp;
						c = c + *c_vec * 2; 
					}
					

				}
			}

			fclose(fid);
			if (chdir(WD) != 0) 
			{
        		perror("chdir a échoué");
    		}

			return c;
}
}





