#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
//#include "surface.h"
#include "hypersurface.h"
#include "local_Support_mM.h"
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
    clock_t start_time = clock();
    // Appeler la fonction de support local
    //Matrix local_support, BF_support, IND_mask_active;
    //local_support = local_support_fun(ELEMENTS, IND_mask, IND_mask_tot, U1, U2, U3, "standard");
    //printMatrix(&IND_mask);
    // Utiliser local_support, BF_support, IND_mask_active comme nécessaire
    //printVector(U1);
    //printMatrix(&IND_mask);

    ListOfVectors local_Support;
    Matrix BF_Support;
    Vector IND_mask_active;
    local_support_fun(p1, p2, p3, n1, n2, n3, DIM, ELEMENTS,IND_mask,IND_mask_tot,U1,U2, U3, &local_Support, &BF_Support, &IND_mask_active);
    clock_t end_time = clock();

    // Calculer la différence pour obtenir le temps écoulé
    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Temps d'exécution : %.4f secondes\n", execution_time);
    if (write_IND_mask_active == 1)
    {
        saveVector(IND_mask_active, "IND_mask_active_c.txt");
    }
    free(IND_mask_active.data);
    if (write_local_support==1)
    {
        saveListOfVectors(local_Support,"local_Support_c.txt");
    }
    
    freeListOfVectors(&local_Support);
    if (write_BF_support==1)
    {
        saveMatrix(BF_Support,"BF_Support_c.txt");
    }
    //
    
    // Libérer la mémoire
    free(ELEMENTS.data);
    free(IND_mask.data);
    free(IND_mask_tot.data);
    free(U1.data);
    free(U2.data);
    free(U3.data);

    return 0;
}