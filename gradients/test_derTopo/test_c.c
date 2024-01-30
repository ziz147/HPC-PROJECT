#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
//#include "surface.h"
#include "hypersurface.h"
#include "local_Support_mM.h"
#include "derTopo.h"
#include <time.h>
#include <unistd.h>

int main() {
    // Charger les données depuis les fichiers
    int DIM, NURBS, length, height, thick;
    int a1_glob, a2_glob, a3_glob, x_div, y_div, z_div;
    int p1, p2, p3, n1, n2, n3;
    int write_BF_support, write_IND_mask_active, write_local_support;

    // Write variables to a .dat file
    FILE *file = fopen("variables.dat", "r");
    file = fopen("variables.dat", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read all values from the file in a single line
    fscanf(file, "DIM: %d\nNURBS: %d\nlength: %d\nheight: %d\nthick: %d\na1_glob: %d\na2_glob: %d\na3_glob: %d\nx_div: %d\ny_div: %d\nz_div: %d\np1: %d\np2: %d\np3: %d\nn1: %d\nn2: %d\nn3: %d\nwrite_BF_support: %d\nwrite_IND_mask_active: %d\nwrite_local_support: %d",
            &DIM, &NURBS, &length, &height, &thick, &a1_glob, &a2_glob, &a3_glob, &x_div, &y_div, &z_div, &p1, &p2, &p3, &n1, &n2, &n3, &write_BF_support, &write_IND_mask_active, &write_local_support);

    fclose(file);
    Matrix ELEMENTS = loadMatrix("ELEMENTS.dat");
    Matrix IND_mask = loadMatrix("IND_mask_test.dat");
    Matrix IND_mask_tot = loadMatrix("IND_mask_tot_test.dat");
    Vector U1 = loadVector("U1.dat");
    Vector U2 = loadVector("U2.dat");
    Vector U3 = loadVector("U3.dat");
    ListOfVectors local_Support;
    COOMatrix BF_Support;
    Vector IND_mask_active;
    fprintf(stderr, " OK\n");
    local_support_fun(p1, p2, p3, n1, n2, n3, DIM, ELEMENTS,IND_mask,IND_mask_tot,U1,U2, U3, &local_Support, &BF_Support, &IND_mask_active);
    free(ELEMENTS.data);
    free(U1.data);
    free(U2.data);
    free(U3.data);
    if (write_IND_mask_active == 1)
    {
        fprintf(stderr, " Sauvegarde IND_mask_active \n");
        saveVector(IND_mask_active, "IND_mask_active_c.txt");
    }
    
    if (write_local_support==1)
    {
        fprintf(stderr, " Sauvegarde local_Support \n");
        saveListOfVectors(local_Support,"local_Support_c.txt");
    }
    
    
    if (write_BF_support==1)
    {
        fprintf(stderr, " Sauvegarde BF_Support \n");
        saveCOOMatrix(&BF_Support,"BF_Support_c.txt");
    }


    
    //Matrix P_rho_temp = loadMatrix("P_rho_test.dat");
    Matrix P_rho= loadMatrix("P_rho_test.dat");

    // il reste a traiter le probleme des matrices 3d et ajouter le depth pour chaque matrice
   // reshape3D(&P_rho_temp, &P_rho, 28, 28, 28);
    //free(P_rho_temp.data);

   // Matrix W_temp = loadMatrix("W_test.dat");
    Matrix W= loadMatrix("W_test.dat");
    if (DIM==3){
        W.cols=28;
        W.rows=28;
        W.depth=28;
        P_rho.cols=28;
        P_rho.rows=28;
        P_rho.depth=28;
    }
    //reshape3D(&W_temp,&W, 28, 28, 28);
   // saveMatrix(W,"der_CP_c.txt");
   // free(W_temp.data);
    Vector rho_e = loadVector("rho_e_test.dat");
    

    COOMatrix der_CP; 
    Vector BF_mask;
    COOMatrix der_W;
    fprintf(stderr, " der nurbs \n");
    int maxind=1;
    der_W.rowsIndices= (int *)malloc( maxind*sizeof(int));
    der_W.colsIndices= (int *)malloc( maxind* sizeof(int));
    der_W.values= (double *)malloc( maxind* sizeof(double));

    der_NURBS(local_Support ,BF_Support ,IND_mask_active , IND_mask, IND_mask_tot ,rho_e, P_rho , W, DIM, &der_CP, &der_W, &BF_mask);
    fprintf(stderr, " NURBS OK \n");
    fprintf(stderr, " Sauvegarde der_W \n");
    saveCOOMatrix(&der_W,"der_W_c.txt");
    fprintf(stderr, " Sauvegarde der_CP \n");
    saveCOOMatrix(&der_CP,"der_CP_c.txt");
    fprintf(stderr, " Sauvegarde BF_mask \n");
    saveVector(BF_mask, "BF_mask_c.txt");
    //
    //COOMatrix der_CP0;
    //der_CP0=convertToCOO(&der_CP);
    //saveCOOMatrix(&der_CP0,"der_CP_c.dat");
    
    
    free(IND_mask_active.data);
    freeListOfVectors(&local_Support);
    //saveVector(BF_mask, "BF_mask_c.txt");
    //saveMatrix(der_CP,"der_CP_c.txt");
    //saveMatrix(der_W,"der_W_c.txt");


    // Libérer la mémoire
    
    //free(IND_mask.data);
   // free(IND_mask_tot.data);

    return 0;
}