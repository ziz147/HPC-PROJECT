#ifndef DER_TOPO_H
#define DER_TOPO_H

#include "matrix.h"  // Assuming you have a separate header for matrix operations

// Function Prototypes
void der_NURBS(ListOfVectors local_support , Matrix BF_support , Vector IND_mask_active , Matrix IND_mask, Matrix IND_mask_tot ,Vector rho_e, Matrix P_rho , Matrix W, int DIM, Vector* der_CP, Vector* der_W, Vector* BF_mask);
Matrix der_BSPLINE(Vector IND_mask_active, Matrix BF_support);

#endif // DER_TOPO_H    