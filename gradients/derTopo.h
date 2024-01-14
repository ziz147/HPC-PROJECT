#ifndef DER_TOPO_H
#define DER_TOPO_H

#include "matrix.h"  // Assuming you have a separate header for matrix operations

// Function Prototypes

Matrix der_BSPLINE(Vector IND_mask_active, Matrix BF_support);
void der_NURBS(ListOfVectors local_support , Matrix BF_support , Vector IND_mask_active , Matrix IND_mask, Matrix IND_mask_tot ,Matrix rho_e, Matrix P_rho , Matrix W, int DIM, Matrix* der_CP, Matrix* der_W, Vector* BF_mask);






// les fonctions utiles:
void SortElements(const Vector *input, Vector *output);

void selectRows(const Matrix *source, const Vector *indices, Matrix *destination);
void selectColumns(const Matrix *source, const Vector *columnIndices, Matrix *destination);
void extractElementsAndReshape(const Matrix *W, const Matrix *indices, Vector *output);
void matrixVectorMultiplication(const Matrix *A, const Vector *B, Vector *C);

#endif // DER_TOPO_H    