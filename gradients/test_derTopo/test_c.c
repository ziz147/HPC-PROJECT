#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
//#include "surface.h"
#include "hypersurface.h"
#include "local_Support_mM.h"
#include "derTopo.h"
#include <time.h>


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
    Matrix IND_mask = loadMatrix("IND_mask.dat");
    Matrix IND_mask_tot = loadMatrix("IND_mask_tot.dat");
    Vector U1 = loadVector("U1.dat");
    Vector U2 = loadVector("U2.dat");
    Vector U3 = loadVector("U3.dat");
    ListOfVectors local_Support;
    Matrix BF_Support;
    Vector IND_mask_active;
    fprintf(stderr, " Ici1");
    local_support_fun(p1, p2, p3, n1, n2, n3, DIM, ELEMENTS,IND_mask,IND_mask_tot,U1,U2, U3, &local_Support, &BF_Support, &IND_mask_active);
    free(ELEMENTS.data);
    free(U1.data);
    free(U2.data);
    free(U3.data);
    fprintf(stderr, " Ici2");
    Matrix P_rho_temp = loadMatrix("P_rho_test.dat");
    Matrix P_rho;
    P_rho=reshape3D(&P_rho_temp, 28, 28, 28);
    free(P_rho_temp.data);

    Matrix W_temp = loadMatrix("W_test.dat");
    Matrix W;
    W=reshape3D(&W_temp, 28, 28, 28);
    free(W_temp.data);
    Vector rho_e = loadVector("rho_e_test.dat");


    Matrix der_CP, der_W; 
    Vector BF_mask;
    der_NURBS(local_Support ,BF_Support ,IND_mask_active , IND_mask, IND_mask_tot ,rho_e, P_rho , W, 2, &der_CP, &der_W, &BF_mask);
    saveVector(BF_mask, "BF_mask_c.txt");
    saveMatrix(der_CP,"der_CP_c.txt");
    saveMatrix(der_W,"der_W_c.txt");


    // Libérer la mémoire
    
    free(IND_mask.data);
    free(IND_mask_tot.data);

    return 0;
}