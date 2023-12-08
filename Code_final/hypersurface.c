#include "hypersurface.h"
#include <stdio.h>
#include <stdlib.h>


// Fonction pour calculer la valeur d'un point sur une hypersurface
double HSurfacePoint_fun(int p, int q, int r , Matrix P, int u1span, int u2span, int u3span, Vector Nu1, Vector Nu2, Vector Nu3) {
    int u1ind = u1span - p;
    double hypersurf = 0.0;

    for (int m = 0; m<= r; m++){
        double  s = 0 ;
        int u3ind = u3span - r + m;
        for (int l = 0; l <= q; l++) {
            double temp = 0.0;
            int u2ind = u2span - q + l;
            for (int k = 0; k <= p; k++) {
                temp += Nu1.data[k] * matrice3d(P,u1ind + k,u2ind,u3ind);
            }
            s = s + Nu2.data[l] * temp;
        }
        hypersurf += Nu3.data[m] * s;
    }
    return hypersurf;
}

// Fonction principale pour calculer l'hypersurface
Matrix HyperSurfacePoint_fun(int n_1, int p , Vector U_1, int n_2 , int q , Vector U_2 , int n_3, int r , Vector U_3, Matrix P, Matrix w, Matrix u_1, Matrix u_2, Matrix u_3, int NURBS ){
    
    Matrix hypersurf;

    // Initialisation de la taille de l'hypersurface
    int size_u_r = u_1.rows;
    int size_u_c = u_1.cols;
    int size_u_t = u_1.depth;
    hypersurf.rows = size_u_r;
    hypersurf.cols= size_u_c;
    hypersurf.depth = size_u_t;
    hypersurf.data = (double *)malloc(size_u_r * size_u_c * size_u_t* sizeof(double));

    // Cas où u est passé en tant que vecteur
    if ( size_u_c == 1 && size_u_t ==1){ // u passed as a vector

        // Initialisation à zéro de l'hypersurface
        for (int i = 0; i <size_u_r ; i++) {
            hypersurf.data[i]=0.;
        }
        if (NURBS == 1){ //NURBS

            // Création de la matrice pondérée p_w
            Matrix p_w;
            p_w.rows = P.rows;
            p_w.cols = P.cols ;
            p_w.depth = P.depth;
            hadamard_product3d(P, w, &p_w);
                // Calcul de l'hypersurface pour chaque point
                for (int i = 0; i < size_u_r; i++) {
                    int u1span , u2span, u3span;
                    Vector nu1, nu2 , nu3;

                    // Trouver les indices de la base et les valeurs de la base pour u_1, u_2 et u_3
                    find_span_basis(matrice3d(u_1, i,0,0), p ,n_1 , U_1 ,&u1span ,&nu1);
                    find_span_basis(matrice3d(u_2, i,0,0), q ,n_2 , U_2 ,&u2span ,&nu2);
                    find_span_basis(matrice3d(u_3, i,0,0), r ,n_3 , U_3 ,&u3span ,&nu3);

                    // Calcul de l'hypersurface pondérée et de la valeur de w
                    double S_cp = HSurfacePoint_fun(p, q, r, p_w, u1span, u2span, u3span, nu1, nu2, nu3);
                    double hs_w = HSurfacePoint_fun(p, q, r, w, u1span, u2span, u3span, nu1, nu2, nu3);

                    // Stockage du résultat dans l'hypersurface
                    hypersurf.data[i] = S_cp / hs_w ;

                    // Libération de la mémoire allouée pour les vecteurs nu1, nu2, nu3
                    free(nu1.data);
                    free(nu2.data);
                    free(nu3.data);
                    
                }
        
        } else { // Bspline

            // Calcul de l'hypersurface pour chaque point
            for (int i = 0; i < size_u_r; i++) {
                    int u1span , u2span, u3span;
                    Vector nu1, nu2 , nu3;

                    // Trouver les indices de la base et les valeurs de la base pour u_1, u_2 et u_3
                    find_span_basis(matrice3d(u_1, i,0,0), p ,n_1 , U_1 ,&u1span ,&nu1);
                    find_span_basis(matrice3d(u_2, i,0,0), q ,n_2 , U_2 ,&u2span ,&nu2);
                    find_span_basis(matrice3d(u_3, i,0,0), r ,n_3 , U_3 ,&u3span ,&nu3);

                    // Calcul de l'hypersurface et stockage dans l'hypersurface
                    hypersurf.data[i] = HSurfacePoint_fun(p, q, r, P, u1span, u2span, u3span, nu1, nu2, nu3);
                    
            }

        }


    } else { // Cas où u est passé en tant que grille
            // Allocation de la mémoire pour l'hypersurface temporaire
        hypersurf.rows = size_u_r; 
        hypersurf.rows= size_u_c;
        hypersurf.depth = size_u_t;
        hypersurf.data = (double *)malloc(size_u_r * size_u_c * size_u_t* sizeof(double));
        Matrix hypersurf_temp;
        hypersurf_temp.rows = size_u_r;
        hypersurf_temp.cols = size_u_c;
        hypersurf_temp.depth = size_u_t;
        hypersurf_temp.data = (double *)malloc(size_u_r * size_u_c * size_u_t* sizeof(double));

        if (NURBS == 1){ //Nurbs
            Matrix p_w;
            p_w.rows = P.rows;
            p_w.cols = P.cols ;
            p_w.depth = P.depth;
            hadamard_product3d(P, w, &p_w); //Hadamard_product3d est défini dans matrix.c

            // Calcul de l'hypersurface pour chaque point de la grille
            for (int i = 0; i < size_u_r; i++) {
                for (int j = 0; j < size_u_c; j++) {
                    for( int k = 0 ; k < size_u_t; k++){
                        int u1span , u2span, u3span;
                        Vector nu1, nu2 , nu3;

                        // Trouver les indices de la base et les valeurs de la base pour u_1, u_2 et u_3
                        find_span_basis(matrice3d(u_1, i,j,k), p ,n_1 , U_1 ,&u1span ,&nu1);
                        find_span_basis(matrice3d(u_2, i,j,k), q ,n_2 , U_2 ,&u2span ,&nu2);
                        find_span_basis(matrice3d(u_3, i,j,k), r ,n_3 , U_3 ,&u3span ,&nu3);

                        // Calcul de l'hypersurface pondérée et de la valeur de w
                        double hs_w  = HSurfacePoint_fun(p, q, r, w, u1span, u2span, u3span, nu1, nu2, nu3);
                        //La matrice 3D est définie dans matrix.c
                        matrice3d(hypersurf_temp, i , j, k)= HSurfacePoint_fun(p, q, r, p_w, u1span, u2span, u3span, nu1, nu2, nu3) /hs_w; 

                       // hypersurf.data[k*size_u_t*size_u_c + j*size_u_c+ i] = matrice3d(hypersurf_temp, i, j,k);
                        matrice3d(hypersurf, i, j, k)= matrice3d(hypersurf_temp, i, j,k);
                    }
                }
            }

        } else { // Bspline

            // Calcul de l'hypersurface pour chaque point de la grille
            for (int i = 0; i < size_u_r; i++) {
                for (int j = 0; j < size_u_c; j++) {
                    for( int k = 0 ; k < size_u_t; k++){
                        int u1span , u2span, u3span;
                        Vector nu1, nu2 , nu3;

                        // Trouver les indices de la base et les valeurs de la base pour u_1, u_2 et u_3
                        find_span_basis(matrice3d(u_1, i,j,k), p ,n_1 , U_1 ,&u1span ,&nu1);
                        find_span_basis(matrice3d(u_2, i,j,k), q ,n_2 , U_2 ,&u2span ,&nu2);
                        find_span_basis(matrice3d(u_3, i,j,k), r ,n_3 , U_3 ,&u3span ,&nu3);

                        // Calcul de l'hypersurface et stockage dans l'hypersurface
                        matrice3d(hypersurf_temp, i , j, k)= HSurfacePoint_fun(p, q, r, P, u1span, u2span, u3span, nu1, nu2, nu3);
                        matrice3d(hypersurf, i, j,k) = matrice3d(hypersurf_temp, i, j,k);
                    }
                }
            }


        }


    }
return hypersurf;
}