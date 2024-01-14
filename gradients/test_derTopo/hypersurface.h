#ifndef HYPERSURFACE_H
#define HYPERSURFACE_H
#include"matrix.h"
#include"surface.h"

double HSurfacePoint_fun(int p, int q, int r ,Matrix P, int u1span, int u2span, int u3span, Vector Nu1, Vector Nu2, Vector Nu3);
Matrix HyperSurfacePoint_fun(int n_1, int p , Vector U_1, int n_2 , int q , Vector U_2 , int n_3, int r , Vector U_3, Matrix P, Matrix w, Matrix u_1, Matrix u_2, Matrix u_3, int NURBS );


#endif