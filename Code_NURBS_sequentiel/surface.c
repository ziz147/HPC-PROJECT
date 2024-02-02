#include "surface.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
void find_span_basis(double pt, int p, int n, const Vector U, int *span, Vector *N) {
    int j, r;
    double saved, temp;
    
    double *left = (double *)malloc((p + 1) * sizeof(double));
    double *right = (double *)malloc((p + 1) * sizeof(double));
    N->data = (double *)malloc((p + 1) * sizeof(double));
    N->length = p + 1;

    
    // Initialization
    for (int i = 0; i < p+1; i++) {
        N->data[i] = 0.0;
        left[i] = 0.0;
        right[i] = 0.0;
    }
    N->data[0] = 1.0;
    *span = 0;

    if (pt == U.data[n + 1]) {
        *span = n;
    } else if (pt <= U.data[p + 1]) {
        *span = p;
    } else {
        int low = p;
        int high = n + 1;
        int mid = (int)((high + low) / 2);

        while (pt < U.data[mid] || pt >= U.data[mid + 1]) {
            if (pt < U.data[mid]) {
                high = mid;
            } else {
                low = mid;
            }
            mid =(int) ((low + high) / 2);
        }

        *span = mid;
    }

    for (j = 1; j <= p; j++) {
        left[j] = pt - U.data[*span + 1 - j];
        right[j] = U.data[*span + j] - pt;
        saved = 0.0;

        for (r = 0; r < j; r++) {
            temp = N->data[r] / (right[r + 1] + left[j - r]);
            N->data[r] = saved + right[r + 1] * temp;
            saved = left[j - r] * temp;
        }
        N->data[j] = saved;
    }

    free(left);
    free(right);
}

double SurfacePoint_fun2(int p, int q, Matrix P, int uspan, int vspan, Vector Nu, Vector Nv) {
    int uind = uspan - p;
    double surf = 0.0;

    for (int l = 0; l <= q; l++) {
        double temp = 0.0;
        int vind = (int)vspan - q + l;

        for (int k = 0; k <= p; k++) {
            temp += Nu.data[k] * matrice(P,uind + k,vind);
            
        }

        surf += Nv.data[l] * temp;
    }

    return surf;
}

Vector SurfacePoint_fun_numba(Matrix u, int n, int p, Vector U, Matrix v, int m, int q, Vector V, Matrix P, Matrix w, int NURBS) {
    Vector surf;
    int size_u_r = u.rows;
    int size_u_c = u.cols;
    surf.length = size_u_r*size_u_c;
    surf.data = (double *)malloc((size_u_r *size_u_c)* sizeof(double));
    int uspan, vspan;

    if (size_u_c == 1) {  // when u is passed as a vector
        
        if (NURBS == 1) {  // NURBS
            Matrix p_w;
            p_w.rows = n+1;
            p_w.cols = m+1;
            p_w.data = (double *)malloc(p_w.rows * p_w.cols * sizeof(double));
            mzero(&p_w);
            //p_w=Mat_mat_product(P,w);
            hadamard_product(&P,&w,&p_w);
            for (int i = 0; i < size_u_r; i++) {
                
                Vector nu, nv;

                find_span_basis(u.data[i], p, n, U, &uspan, &nu);
                find_span_basis(v.data[i], q, m, V, &vspan, &nv);

                double surf_cp = SurfacePoint_fun2(p, q, p_w, uspan, vspan, nu, nv);
                double surf_w = SurfacePoint_fun2(p, q, w, uspan, vspan, nu, nv);
                surf.data[i] = surf_cp / surf_w;
                free(nu.data);
                free(nv.data);
            }

            free(p_w.data);
        } else {  // BSPLINE
            for (int i = 0; i < size_u_r; i++) {
                
                Vector nu, nv;

                find_span_basis(u.data[i], p, n, U, &uspan, &nu);


                find_span_basis(v.data[i], q, m, V, &vspan, &nv);


                double surf_matrix = SurfacePoint_fun2(p, q, P, uspan, vspan, nu, nv);

                surf.data[i] = surf_matrix;
                free(nu.data);
                free(nv.data);
            }
        }
    } else {  // when u is passed as a grid
        
        Matrix surf_temp;
        surf_temp.rows = size_u_r;
        surf_temp.cols = size_u_c;
        surf_temp.data = (double *)malloc(size_u_r * size_u_c * sizeof(double));

        if (NURBS == 1) {  // NURBS
            Matrix p_w;
            p_w.rows = n + 1;
            p_w.cols = m + 1;
            p_w.data = (double *)malloc(p_w.rows * p_w.cols * sizeof(double));
            //p_w=Mat_mat_product(P,w);
            hadamard_product(&P,&w,&p_w);
            for (int i = 0; i < size_u_r; i++) {
                for (int j = 0; j < size_u_c; j++) {
                    
                    Vector nu, nv;

                    find_span_basis(u.data[i * size_u_c + j], p, n, U, &uspan, &nu);
                    find_span_basis(v.data[i * size_u_c + j], q, m, V, &vspan, &nv);

                    double c_w = SurfacePoint_fun2(p, q, w, uspan, vspan, nu, nv);
                    double surf_cp = SurfacePoint_fun2(p, q, p_w, uspan, vspan, nu, nv);
                    matrice(surf_temp,i,j) = surf_cp / c_w;
                    free(nu.data);
                    free(nv.data);
                }
            }

            free(p_w.data);
        } else {  // BSPLINE
            for (int i = 0; i < size_u_r; i++) {
                for (int j = 0; j < size_u_c; j++) {
                    
                    Vector nu, nv;

                    find_span_basis(u.data[i * size_u_c + j], p, n, U, &uspan, &nu);
                    find_span_basis(v.data[i * size_u_c + j], q, m, V, &vspan, &nv);

                    surf_temp.data[i * size_u_c + j] = SurfacePoint_fun2(p, q, P, uspan, vspan, nu, nv);
                    free(nu.data);
                    free(nv.data);
                }
            }
        }
        for (int i = 0; i < size_u_r * size_u_c; i++) {
            surf.data[i] = surf_temp.data[i];
        }
    }

    return surf;
}
