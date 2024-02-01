#ifndef MYHEADER_H
#define MYHEADER_H

#include "matrix.h"
//#include "surface.h"
#include "hypersurface.h"


// Structure for integer list
// Function to initialize an integer list
// Function for ls_2d_numba
void ls_2d_numba(Matrix IND_mask_tot, Matrix IND_mask, Matrix u1, Matrix u2, Vector U1, Vector U2, int p1_temp, int p2_temp, int n1_temp, int n2_temp, ListOfVectors *local_Support, COOMatrix *BF_Support, Vector *IND_mask_active);

void ls_3d(Matrix IND_mask_tot, Matrix IND_mask, Matrix u1, Matrix u2,Matrix u3 , Vector U1,  Vector U2, Vector U3, int p1_temp, int p2_temp, int p3_temp, int n1_temp, int n2_temp, int n3_temp, ListOfVectors *local_Support, COOMatrix *BF_Support, Vector *IND_mask_active);

void local_support_fun(int p1, int p2, int p3, int n1, int n2, int n3, int DIM, Matrix ELEMENTS, Matrix IND_mask, Matrix IND_mask_tot, Vector U1, Vector U2, Vector U3, ListOfVectors *local_Support, COOMatrix *BF_Support, Vector *IND_mask_active);


//void local_support_fun(Matrix ELEMENTS, Matrix IND_mask, Matrix IND_mask_tot, Vector U1, Vector U2, Vector U3, char *flag_scale, ListOfVectors *local_Support, Matrix *BF_Support, Vector *IND_mask_active);

#endif // MYHEADER_H
