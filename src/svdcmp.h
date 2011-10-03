// ------------------------------------------------------------------------
// An implementation of SVD from Numerical Recipes in C and Mike's lectures
// ------------------------------------------------------------------------

#ifndef SVDCMP_H
#define SVDCMP_H

// ------------------------------------------------------------------------
// Modified from Numerical Recipes in C
// Given a matrix a[nRows][nCols], svdcmp() computes its singular value 
// decomposition, A = U * W * Vt.  A is replaced by U when svdcmp 
// returns.  The diagonal matrix W is output as a vector w[nCols].
// V (not V transpose) is output as the matrix V[nCols][nCols].
// CAUTION : Output is unsorted!!!!!
// ------------------------------------------------------------------------
extern int svdcmp(double **a, int nRows, int nCols, double *w, double **v);
extern int svdcmp(double **a, int nRows, int nCols, double *w, double **v, double* rv1);

// ------------------------------------------------------------------------
// calculates sqrt( a^2 + b^2 ) with decent precision
// ------------------------------------------------------------------------
extern double pythag(double a, double b);

#endif
