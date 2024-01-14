#ifndef VOLUME_H
#define VOLUME_H
#include "matrix.h"

void volume_grad_fun(Vector rho_e, Matrix P_rho, Matrix W, Matrix ELEMENTS , Matrix IND_mask ,ListOfVectors local_support,Matrix BF_supprt, Matrix IND_mask_tot, Vector IND_mask_active,int DERIVATIVES, int DIM,int NURBS ,Vector *grad_v);


#endif

