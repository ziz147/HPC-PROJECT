#include "matrix.h"
#include "compliance2.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

int main() 
{
    printf("error ici");
    //Création de vecteurs pour tester la fonction
    Vector U, strain_M;

    // // Exemple de données d'entrée
    double U_data[] = {1.0, 2.0, 3.0};
    double strain_M_data[] = {10.0, 20.0, 30.0};
    

    U.length = 3;
    U.data = U_data;

    strain_M.length = 3;
    strain_M.data = strain_M_data;

    // // Appel de la fonction à tester
    rearrangeStrainM(U, strain_M);

    // // Vérification des résultats
    // double expected_strain_M[] = {10.0, 20.0, 30.0};  // La sortie attendue après réarrangement
    // for (int i = 0; i < strain_M.length; i++) {
    //     assert(strain_M.data[i] == expected_strain_M[i]);
    // }

    // printf("Test réussi !\n");

    return 0;
}
