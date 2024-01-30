#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H
#include <omp.h>
typedef struct {
    double *data;
    int rows;
    int cols;
    int depth;
} Matrix;

typedef struct {
    double *values;   // Tableau des valeurs non nulles
    int *rowsIndices; // Indices de ligne pour chaque valeur non nulle
    int *colsIndices; // Indices de colonne pour chaque valeur non nulle
    int *depthsIndices; // Indices de profondeur pour chaque valeur non nulle
    int nonZeroCount; // Nombre d'éléments non nuls
    int rows;         // Nombre total de lignes de la matrice d'origine
    int cols;         // Nombre total de colonnes de la matrice d'origine
    int depth;        // Nombre total de profondeurs de la matrice d'origine
} COOMatrix;



#define GET_COO_VALUE(cooMat, rowIdx, colIdx) ({ \
    double val = 0.0; \
    for (int i = 0; i < (cooMat).nonZeroCount; ++i) { \
        if ((cooMat).rowsIndices[i] == (rowIdx) && (cooMat).colsIndices[i] == (colIdx) ) { \
            val = (cooMat).values[i]; \
            break; \
        } \
    } \
    val; \
})











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
typedef struct {
    double *values;       // Tableau des valeurs non nulles
    int *indices;         // Tableau des indices des valeurs non nulles
    int nonZeroCount;     // Nombre d'éléments non nuls
    int originalLength;   // Taille originale du vecteur
} SparseVector;

typedef struct {
    SparseVector *vectors; // Tableau de vecteurs creux
    int size;              // Nombre de vecteurs creux
} ListOfSparseVectors;
#define GET_SPARSE_VALUE(sparseVec, idx) ({ \
    double val = 0.0; \
    for (int k = 0; k < (sparseVec).nonZeroCount; ++k) { \
        if ((sparseVec).indices[k] == idx) { \
            val = (sparseVec).values[k]; \
            break; \
        } \
    } \
    val; \
})

#define matrice(matrix, i, j) ((matrix).data[(i) * (matrix).cols + (j)])
#define matricep(matrix, i, j) ((matrix)->data[(i) * (matrix)->cols + (j)])
#define matrice3d(matrix, i, j, k) ((matrix).data[(i) * ((matrix).cols * (matrix).depth) + (j) * (matrix).depth + (k)])
#define matrice3dp(matrix, i, j, k) ((matrix)->data[(i) * ((matrix)->cols * (matrix)->depth) + (j) * (matrix)->depth + (k)])


Matrix loadMatrix(const char *filename);
Vector loadVector(const char *filename);
void saveCOOMatrix(const COOMatrix *matrix, const char *filename);
COOMatrix *loadCOOMatrix(const char *filename);
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
Vector extractValues(Matrix BF_Support, int* ind, int k, int size);
void saveMatrix(Matrix matrix, const char *filename);
void saveListOfVectors( ListOfVectors listOfVectors, const char *filename);
Matrix col3d(const Matrix matrix, int j);
void freeListOfVectors(ListOfVectors *list);
Vector Mat_vec_product(Matrix A, Vector B);
Matrix Mat_mat_product(Matrix A, Matrix B);
COOMatrix convertToCOO(Matrix *denseMatrix);
Matrix convertToDense(COOMatrix *cooMatrix);
void hadamardVectorProductCOO(const COOMatrix *P, const Vector *w, COOMatrix *p_w);
void reshape3D(Matrix *matrix2D,Matrix *matrix3D, int newRows, int newCols, int newDepth);
SparseVector convertToSparseVector(Vector *vector);
ListOfSparseVectors convertToListOfSparseVectors(ListOfVectors *list);
void copyCOOMatrixStructure(const COOMatrix *source, COOMatrix *destination);
void freeCOOMatrix(COOMatrix *matrix);
void hadamardVectorProductCOO2(const COOMatrix *P, const Vector *w, COOMatrix *p_w,Vector max);
void addValueToCOOMatrix(COOMatrix *matrix, int row, int col, double value);
#endif