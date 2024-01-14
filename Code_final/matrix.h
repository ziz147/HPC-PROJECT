#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

typedef struct {
    double *data;
    int rows;
    int cols;
    int depth;
} Matrix;

typedef struct 
{
    double *data;
    int *rows;
    int *cols;
    int nnz ; //Nombre d'éléments non nuls

} COOMatrix ;

#define initmatrice3d(matrix, d1, d2, d3) \
    do { \
        (matrix).rows = (d1); \
        (matrix).cols = (d2); \
        (matrix).depth = (d3); \
        int size = (d1) * (d2) * (d3); \
        (matrix).data = (double *)malloc(size * sizeof(double)); \
        for (int i = 0; i < size; ++i) { \
            (matrix).data[i] = 0.0; \
        } \
    } while (0)

#define initmatrice3dones(matrix, d1, d2, d3) \
    do { \
        (matrix).rows = (d1); \
        (matrix).cols = (d2); \
        (matrix).depth = (d3); \
        int size = (d1) * (d2) * (d3); \
        (matrix).data = (double *)malloc(size * sizeof(double)); \
        for (int i = 0; i < size; ++i) { \
            (matrix).data[i] = 1.0; \
        } \
    } while (0)
typedef struct {
    double *data;
    int length;
} Vector;
typedef struct {
    Vector *vectors; // Tableau de vecteurs d'indices
    int size;     // Nombre de vecteurs
} ListOfVectors;

#define matrice(matrix, i, j) ((matrix).data[(i) * (matrix).cols + (j)])
#define matricep(matrix, i, j) ((matrix)->data[(i) * (matrix)->cols + (j)])
#define matrice3d(matrix, i, j, k) ((matrix).data[(i) * ((matrix).cols * (matrix).depth) + (j) * (matrix).depth + (k)])


Matrix loadMatrix(const char *filename);
Vector loadVector(const char *filename);
COOMatrix loadCOO(const char *filename);
void printVector(Vector vector);
void printMatrix(const Matrix matrix);
Vector col(const Matrix matrix, int i);
Vector line(const Matrix matrix, int i); 
void hadamard_vector_product(const Matrix *P, const Vector *w, Matrix *p_w);
void hadamard_product(const Matrix *P, const Matrix *w, Matrix *p_w);
void hadamard_product3d(const Matrix P, const Matrix w, Matrix *p_w);
void definem(Matrix* M, int i, int j);
void initializeMatrix(Matrix *mat,int n,int m);
void initializeVecteur(Vector *vec,int n);
void mzero(Matrix *mat);
void saveVector(Vector vector, const char *filename);
void addVectorToList(ListOfVectors *listOfVectors, int vectorLength);

void initVector(Vector *vector, int length);
Vector extractValues(Matrix BF_Support, int* ind, int k, int size);
void saveMatrix(Matrix matrix, const char *filename);
void saveCOO(COOMatrix coo, const char *filename);
void saveListOfVectors( ListOfVectors listOfVectors, const char *filename);
Matrix col3d(const Matrix matrix, int j);
void freeListOfVectors(ListOfVectors *list);
Vector Mat_vec_product(Matrix A, Vector B);
COOMatrix denseToCOO(Matrix matrix);
Matrix COOtoDense(COOMatrix coo, int rows, int cols);
#endif