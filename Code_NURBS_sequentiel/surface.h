#ifndef SURFACE_H
#define SURFACE_H

#include "matrix.h"
void find_span_basis(double pt, int p, int n, const Vector U, int *span, Vector *N);
double SurfacePoint_fun2(int p, int q, Matrix P, int uspan, int vspan, Vector Nu, Vector Nv);
Vector SurfacePoint_fun_numba(Matrix u, int n, int p, Vector U, Matrix v, int m, int q, Vector V, Matrix P, Matrix w, int NURBS);

#endif