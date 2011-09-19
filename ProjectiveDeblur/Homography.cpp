#include "homography.h"
#include "svdcmp.h"

void Homography::ComputeHomography(double correspondants[4][4]){
	int k;
	double sx1,sy1,sx2,sy2;

/////////////////////////////////////////////////////////////
// Map correspondants to feature
////////////////////////////////////////////////////////////
	double** featurevector;
	featurevector = new double*[8];
	for(k = 0; k < 4; k++){
		featurevector[2*k] = new double[9];
		featurevector[2*k+1] = new double[9];
		sx1 = correspondants[k][0];
		sy1 = correspondants[k][1];
		sx2 = correspondants[k][2];
		sy2 = correspondants[k][3];
		featurevector[2*k][0] = sx1;
		featurevector[2*k][1] = sy1;
		featurevector[2*k][2] = 1;
		featurevector[2*k][3] = 0;
		featurevector[2*k][4] = 0;
		featurevector[2*k][5] = 0;
		featurevector[2*k][6] = -sx2 * sx1;
		featurevector[2*k][7] = -sx2 * sy1;
		featurevector[2*k][8] = -sx2;

		featurevector[2*k+1][0] = 0;
		featurevector[2*k+1][1] = 0;
		featurevector[2*k+1][2] = 0;
		featurevector[2*k+1][3] = sx1;
		featurevector[2*k+1][4] = sy1;
		featurevector[2*k+1][5] = 1;
		featurevector[2*k+1][6] = -sy2 * sx1;
		featurevector[2*k+1][7] = -sy2 * sy1;
		featurevector[2*k+1][8] = -sy2;
	}

/////////////////////////////////////////////////////////////
// Least square distance method
////////////////////////////////////////////////////////////
	double* w = new double[9];
	double** v = new double*[9];
	for(k = 0; k < 9; k++) v[k] = new double[9];

	int minwindex;
	double minw;

	svdcmp(featurevector, 8, 9, w, v);

	minwindex = 0;
	minw = w[0];
	for(k = 1; k < 9; k++){
		if(minw > w[k]){	minw = w[k];	minwindex = k;	}
	}

	Hmatrix[0][0] = (float)(v[0][minwindex]);
	Hmatrix[0][1] = (float)(v[1][minwindex]);
	Hmatrix[0][2] = (float)(v[2][minwindex]);
	Hmatrix[1][0] = (float)(v[3][minwindex]);
	Hmatrix[1][1] = (float)(v[4][minwindex]);
	Hmatrix[1][2] = (float)(v[5][minwindex]);
	Hmatrix[2][0] = (float)(v[6][minwindex]);
	Hmatrix[2][1] = (float)(v[7][minwindex]);
	Hmatrix[2][2] = (float)(v[8][minwindex]);

	for(k = 0; k < 8; k++) delete [] featurevector[k];
	delete [] featurevector;
	for(k = 0; k < 9; k++) delete [] v[k];
	delete [] v;
	delete [] w;
}

void Homography::ComputeHomography(double correspondantsA[4][2], double correspondantsB[4][2]){
	int k;
	double sx1,sy1,sx2,sy2;

/////////////////////////////////////////////////////////////
// Map correspondants to feature
////////////////////////////////////////////////////////////
	double** featurevector;
	featurevector = new double*[8];
	for(k = 0; k < 4; k++){
		featurevector[2*k] = new double[9];
		featurevector[2*k+1] = new double[9];
		sx1 = correspondantsA[k][0];
		sy1 = correspondantsA[k][1];
		sx2 = correspondantsB[k][0];
		sy2 = correspondantsB[k][1];
		featurevector[2*k][0] = sx1;
		featurevector[2*k][1] = sy1;
		featurevector[2*k][2] = 1;
		featurevector[2*k][3] = 0;
		featurevector[2*k][4] = 0;
		featurevector[2*k][5] = 0;
		featurevector[2*k][6] = -sx2 * sx1;
		featurevector[2*k][7] = -sx2 * sy1;
		featurevector[2*k][8] = -sx2;

		featurevector[2*k+1][0] = 0;
		featurevector[2*k+1][1] = 0;
		featurevector[2*k+1][2] = 0;
		featurevector[2*k+1][3] = sx1;
		featurevector[2*k+1][4] = sy1;
		featurevector[2*k+1][5] = 1;
		featurevector[2*k+1][6] = -sy2 * sx1;
		featurevector[2*k+1][7] = -sy2 * sy1;
		featurevector[2*k+1][8] = -sy2;
	}

/////////////////////////////////////////////////////////////
// Least square distance method
////////////////////////////////////////////////////////////
	double* w = new double[9];
	double** v = new double*[9];
	for(k = 0; k < 9; k++) v[k] = new double[9];

	int minwindex;
	double minw;

	svdcmp(featurevector, 8, 9, w, v);

	minwindex = 0;
	minw = w[0];
	for(k = 1; k < 9; k++){
		if(minw > w[k]){	minw = w[k];	minwindex = k;	}
	}

	Hmatrix[0][0] = (float)(v[0][minwindex]);
	Hmatrix[0][1] = (float)(v[1][minwindex]);
	Hmatrix[0][2] = (float)(v[2][minwindex]);
	Hmatrix[1][0] = (float)(v[3][minwindex]);
	Hmatrix[1][1] = (float)(v[4][minwindex]);
	Hmatrix[1][2] = (float)(v[5][minwindex]);
	Hmatrix[2][0] = (float)(v[6][minwindex]);
	Hmatrix[2][1] = (float)(v[7][minwindex]);
	Hmatrix[2][2] = (float)(v[8][minwindex]);

	for(k = 0; k < 8; k++) delete [] featurevector[k];
	delete [] featurevector;
	for(k = 0; k < 9; k++) delete [] v[k];
	delete [] v;
	delete [] w;
}

void Homography::ComputeHomography(double correspondantsA[4][2], double correspondantsB[4][2], double** featurevector, double* w, double** v, double* rv1){
	int k;
	double sx1,sy1,sx2,sy2;

/////////////////////////////////////////////////////////////
// Map correspondants to feature
////////////////////////////////////////////////////////////
	for(k = 0; k < 4; k++){
		sx1 = correspondantsA[k][0];
		sy1 = correspondantsA[k][1];
		sx2 = correspondantsB[k][0];
		sy2 = correspondantsB[k][1];
		featurevector[2*k][0] = sx1;
		featurevector[2*k][1] = sy1;
		featurevector[2*k][2] = 1;
		featurevector[2*k][3] = 0;
		featurevector[2*k][4] = 0;
		featurevector[2*k][5] = 0;
		featurevector[2*k][6] = -sx2 * sx1;
		featurevector[2*k][7] = -sx2 * sy1;
		featurevector[2*k][8] = -sx2;

		featurevector[2*k+1][0] = 0;
		featurevector[2*k+1][1] = 0;
		featurevector[2*k+1][2] = 0;
		featurevector[2*k+1][3] = sx1;
		featurevector[2*k+1][4] = sy1;
		featurevector[2*k+1][5] = 1;
		featurevector[2*k+1][6] = -sy2 * sx1;
		featurevector[2*k+1][7] = -sy2 * sy1;
		featurevector[2*k+1][8] = -sy2;
	}

/////////////////////////////////////////////////////////////
// Least square distance method
////////////////////////////////////////////////////////////
	int minwindex;
	double minw;

	svdcmp(featurevector, 8, 9, w, v, rv1);

	minwindex = 0;
	minw = w[0];
	for(k = 1; k < 9; k++){
		if(minw > w[k]){	minw = w[k];	minwindex = k;	}
	}

	Hmatrix[0][0] = (float)(v[0][minwindex]);
	Hmatrix[0][1] = (float)(v[1][minwindex]);
	Hmatrix[0][2] = (float)(v[2][minwindex]);
	Hmatrix[1][0] = (float)(v[3][minwindex]);
	Hmatrix[1][1] = (float)(v[4][minwindex]);
	Hmatrix[1][2] = (float)(v[5][minwindex]);
	Hmatrix[2][0] = (float)(v[6][minwindex]);
	Hmatrix[2][1] = (float)(v[7][minwindex]);
	Hmatrix[2][2] = (float)(v[8][minwindex]);
}

void Homography::ComputeHomography(double** correspondants, int ncor){
	int k;
	double sx1,sy1,sx2,sy2;

/////////////////////////////////////////////////////////////
// Map correspondants to feature
////////////////////////////////////////////////////////////
	double** featurevector;
	featurevector = new double*[2*ncor];
	for(k = 0; k < ncor; k++){
		featurevector[2*k] = new double[9];
		featurevector[2*k+1] = new double[9];
		sx1 = correspondants[k][0];
		sy1 = correspondants[k][1];
		sx2 = correspondants[k][2];
		sy2 = correspondants[k][3];
		featurevector[2*k][0] = sx1;
		featurevector[2*k][1] = sy1;
		featurevector[2*k][2] = 1;
		featurevector[2*k][3] = 0;
		featurevector[2*k][4] = 0;
		featurevector[2*k][5] = 0;
		featurevector[2*k][6] = -sx2 * sx1;
		featurevector[2*k][7] = -sx2 * sy1;
		featurevector[2*k][8] = -sx2;

		featurevector[2*k+1][0] = 0;
		featurevector[2*k+1][1] = 0;
		featurevector[2*k+1][2] = 0;
		featurevector[2*k+1][3] = sx1;
		featurevector[2*k+1][4] = sy1;
		featurevector[2*k+1][5] = 1;
		featurevector[2*k+1][6] = -sy2 * sx1;
		featurevector[2*k+1][7] = -sy2 * sy1;
		featurevector[2*k+1][8] = -sy2;
	}

/////////////////////////////////////////////////////////////
// Least square distance method
////////////////////////////////////////////////////////////
	double* w = new double[9];
	double** v = new double*[9];
	for(k = 0; k < 9; k++) v[k] = new double[9];

	int minwindex;
	double minw;

	svdcmp(featurevector, 2*ncor, 9, w, v);

	minwindex = 0;
	minw = w[0];
	for(k = 1; k < 9; k++){
		if(minw > w[k]){	minw = w[k];	minwindex = k;	}
	}

	Hmatrix[0][0] = (float)(v[0][minwindex]);
	Hmatrix[0][1] = (float)(v[1][minwindex]);
	Hmatrix[0][2] = (float)(v[2][minwindex]);
	Hmatrix[1][0] = (float)(v[3][minwindex]);
	Hmatrix[1][1] = (float)(v[4][minwindex]);
	Hmatrix[1][2] = (float)(v[5][minwindex]);
	Hmatrix[2][0] = (float)(v[6][minwindex]);
	Hmatrix[2][1] = (float)(v[7][minwindex]);
	Hmatrix[2][2] = (float)(v[8][minwindex]);

	for(k = 0; k < 2*ncor; k++) delete [] featurevector[k];
	delete [] featurevector;
	for(k = 0; k < 9; k++) delete [] v[k];
	delete [] v;
	delete [] w;
}

void Homography::ComputeHomography(double** correspondantsA, double** correspondantsB, int ncor){
	int k;
	double sx1,sy1,sx2,sy2;

/////////////////////////////////////////////////////////////
// Map correspondants to feature
////////////////////////////////////////////////////////////
	double** featurevector;
	featurevector = new double*[2*ncor];
	for(k = 0; k < ncor; k++){
		featurevector[2*k] = new double[9];
		featurevector[2*k+1] = new double[9];
		sx1 = correspondantsA[k][0];
		sy1 = correspondantsA[k][1];
		sx2 = correspondantsB[k][0];
		sy2 = correspondantsB[k][1];
		featurevector[2*k][0] = sx1;
		featurevector[2*k][1] = sy1;
		featurevector[2*k][2] = 1;
		featurevector[2*k][3] = 0;
		featurevector[2*k][4] = 0;
		featurevector[2*k][5] = 0;
		featurevector[2*k][6] = -sx2 * sx1;
		featurevector[2*k][7] = -sx2 * sy1;
		featurevector[2*k][8] = -sx2;

		featurevector[2*k+1][0] = 0;
		featurevector[2*k+1][1] = 0;
		featurevector[2*k+1][2] = 0;
		featurevector[2*k+1][3] = sx1;
		featurevector[2*k+1][4] = sy1;
		featurevector[2*k+1][5] = 1;
		featurevector[2*k+1][6] = -sy2 * sx1;
		featurevector[2*k+1][7] = -sy2 * sy1;
		featurevector[2*k+1][8] = -sy2;
	}

/////////////////////////////////////////////////////////////
// Least square distance method
////////////////////////////////////////////////////////////
	double* w = new double[9];
	double** v = new double*[9];
	for(k = 0; k < 9; k++) v[k] = new double[9];

	int minwindex;
	double minw;

	svdcmp(featurevector, 2*ncor, 9, w, v);

	minwindex = 0;
	minw = w[0];
	for(k = 1; k < 9; k++){
		if(minw > w[k]){	minw = w[k];	minwindex = k;	}
	}

	Hmatrix[0][0] = (float)(v[0][minwindex]);
	Hmatrix[0][1] = (float)(v[1][minwindex]);
	Hmatrix[0][2] = (float)(v[2][minwindex]);
	Hmatrix[1][0] = (float)(v[3][minwindex]);
	Hmatrix[1][1] = (float)(v[4][minwindex]);
	Hmatrix[1][2] = (float)(v[5][minwindex]);
	Hmatrix[2][0] = (float)(v[6][minwindex]);
	Hmatrix[2][1] = (float)(v[7][minwindex]);
	Hmatrix[2][2] = (float)(v[8][minwindex]);

	for(k = 0; k < 2*ncor; k++) delete [] featurevector[k];
	delete [] featurevector;
	for(k = 0; k < 9; k++) delete [] v[k];
	delete [] v;
	delete [] w;
}

void Homography::ComputeAffineHomography(double correspondants[4][4]){
	int k;
	double sx1,sy1,sx2,sy2;

/////////////////////////////////////////////////////////////
// Map correspondants to feature
////////////////////////////////////////////////////////////
	double** featurevector;
	featurevector = new double*[8];
	for(k = 0; k < 4; k++){
		featurevector[2*k] = new double[7];
		featurevector[2*k+1] = new double[7];
		sx1 = correspondants[k][0];
		sy1 = correspondants[k][1];
		sx2 = correspondants[k][2];
		sy2 = correspondants[k][3];
		featurevector[2*k][0] = sx1;
		featurevector[2*k][1] = sy1;
		featurevector[2*k][2] = 1;
		featurevector[2*k][3] = 0;
		featurevector[2*k][4] = 0;
		featurevector[2*k][5] = 0;
		featurevector[2*k][6] = -sx2;

		featurevector[2*k+1][0] = 0;
		featurevector[2*k+1][1] = 0;
		featurevector[2*k+1][2] = 0;
		featurevector[2*k+1][3] = sx1;
		featurevector[2*k+1][4] = sy1;
		featurevector[2*k+1][5] = 1;
		featurevector[2*k+1][6] = -sy2;
	}

/////////////////////////////////////////////////////////////
// Least square distance method
////////////////////////////////////////////////////////////
	double* w = new double[7];
	double** v = new double*[7];
	for(k = 0; k < 7; k++) v[k] = new double[7];

	int minwindex;
	double minw;

	svdcmp(featurevector, 8, 7, w, v);

	minwindex = 0;
	minw = w[0];
	for(k = 1; k < 7; k++){
		if(minw > w[k]){	minw = w[k];	minwindex = k;	}
	}

	Hmatrix[0][0] = (float)(v[0][minwindex] / v[6][minwindex]);
	Hmatrix[0][1] = (float)(v[1][minwindex] / v[6][minwindex]);
	Hmatrix[0][2] = (float)(v[2][minwindex] / v[6][minwindex]);
	Hmatrix[1][0] = (float)(v[3][minwindex] / v[6][minwindex]);
	Hmatrix[1][1] = (float)(v[4][minwindex] / v[6][minwindex]);
	Hmatrix[1][2] = (float)(v[5][minwindex] / v[6][minwindex]);
	Hmatrix[2][0] = 0.0f;
	Hmatrix[2][1] = 0.0f;
	Hmatrix[2][2] = 1.0f;

	for(k = 0; k < 8; k++) delete [] featurevector[k];
	delete [] featurevector;
	for(k = 0; k < 7; k++) delete [] v[k];
	delete [] v;
	delete [] w;
}

void Homography::ComputeAffineHomography(double correspondantsA[4][2], double correspondantsB[4][2]){
	int k;
	double sx1,sy1,sx2,sy2;

/////////////////////////////////////////////////////////////
// Map correspondants to feature
////////////////////////////////////////////////////////////
	double** featurevector;
	featurevector = new double*[8];
	for(k = 0; k < 4; k++){
		featurevector[2*k] = new double[7];
		featurevector[2*k+1] = new double[7];
		sx1 = correspondantsA[k][0];
		sy1 = correspondantsA[k][1];
		sx2 = correspondantsB[k][0];
		sy2 = correspondantsB[k][1];
		featurevector[2*k][0] = sx1;
		featurevector[2*k][1] = sy1;
		featurevector[2*k][2] = 1;
		featurevector[2*k][3] = 0;
		featurevector[2*k][4] = 0;
		featurevector[2*k][5] = 0;
		featurevector[2*k][6] = -sx2;

		featurevector[2*k+1][0] = 0;
		featurevector[2*k+1][1] = 0;
		featurevector[2*k+1][2] = 0;
		featurevector[2*k+1][3] = sx1;
		featurevector[2*k+1][4] = sy1;
		featurevector[2*k+1][5] = 1;
		featurevector[2*k+1][6] = -sy2;
	}

/////////////////////////////////////////////////////////////
// Least square distance method
////////////////////////////////////////////////////////////
	double* w = new double[7];
	double** v = new double*[7];
	for(k = 0; k < 7; k++) v[k] = new double[7];

	int minwindex;
	double minw;

	svdcmp(featurevector, 8, 7, w, v);

	minwindex = 0;
	minw = w[0];
	for(k = 1; k < 7; k++){
		if(minw > w[k]){	minw = w[k];	minwindex = k;	}
	}

	Hmatrix[0][0] = (float)(v[0][minwindex] / v[6][minwindex]);
	Hmatrix[0][1] = (float)(v[1][minwindex] / v[6][minwindex]);
	Hmatrix[0][2] = (float)(v[2][minwindex] / v[6][minwindex]);
	Hmatrix[1][0] = (float)(v[3][minwindex] / v[6][minwindex]);
	Hmatrix[1][1] = (float)(v[4][minwindex] / v[6][minwindex]);
	Hmatrix[1][2] = (float)(v[5][minwindex] / v[6][minwindex]);
	Hmatrix[2][0] = 0.0f;
	Hmatrix[2][1] = 0.0f;
	Hmatrix[2][2] = 1.0f;

	for(k = 0; k < 8; k++) delete [] featurevector[k];
	delete [] featurevector;
	for(k = 0; k < 7; k++) delete [] v[k];
	delete [] v;
	delete [] w;
}

void Homography::ComputeAffineHomography(double** correspondants, int ncor){
	int k;
	double sx1,sy1,sx2,sy2;

/////////////////////////////////////////////////////////////
// Map correspondants to feature
////////////////////////////////////////////////////////////
	double** featurevector;
	featurevector = new double*[2*ncor];
	for(k = 0; k < ncor; k++){
		featurevector[2*k] = new double[7];
		featurevector[2*k+1] = new double[7];
		sx1 = correspondants[k][0];
		sy1 = correspondants[k][1];
		sx2 = correspondants[k][2];
		sy2 = correspondants[k][3];
		featurevector[2*k][0] = sx1;
		featurevector[2*k][1] = sy1;
		featurevector[2*k][2] = 1;
		featurevector[2*k][3] = 0;
		featurevector[2*k][4] = 0;
		featurevector[2*k][5] = 0;
		featurevector[2*k][6] = -sx2;

		featurevector[2*k+1][0] = 0;
		featurevector[2*k+1][1] = 0;
		featurevector[2*k+1][2] = 0;
		featurevector[2*k+1][3] = sx1;
		featurevector[2*k+1][4] = sy1;
		featurevector[2*k+1][5] = 1;
		featurevector[2*k+1][6] = -sy2;
	}

/////////////////////////////////////////////////////////////
// Least square distance method
////////////////////////////////////////////////////////////
	double* w = new double[7];
	double** v = new double*[7];
	for(k = 0; k < 7; k++) v[k] = new double[7];

	int minwindex;
	double minw;

	svdcmp(featurevector, 2*ncor, 7, w, v);

	minwindex = 0;
	minw = w[0];
	for(k = 1; k < 7; k++){
		if(minw > w[k]){	minw = w[k];	minwindex = k;	}
	}

	Hmatrix[0][0] = (float)(v[0][minwindex] / v[6][minwindex]);
	Hmatrix[0][1] = (float)(v[1][minwindex] / v[6][minwindex]);
	Hmatrix[0][2] = (float)(v[2][minwindex] / v[6][minwindex]);
	Hmatrix[1][0] = (float)(v[3][minwindex] / v[6][minwindex]);
	Hmatrix[1][1] = (float)(v[4][minwindex] / v[6][minwindex]);
	Hmatrix[1][2] = (float)(v[5][minwindex] / v[6][minwindex]);
	Hmatrix[2][0] = 0.0f;
	Hmatrix[2][1] = 0.0f;
	Hmatrix[2][2] = 1.0f;

	for(k = 0; k < 2*ncor; k++) delete [] featurevector[k];
	delete [] featurevector;
	for(k = 0; k < 7; k++) delete [] v[k];
	delete [] v;
	delete [] w;
}

void Homography::ComputeAffineHomography(double** correspondantsA, double** correspondantsB, int ncor){
	int k;
	double sx1,sy1,sx2,sy2;

/////////////////////////////////////////////////////////////
// Map correspondants to feature
////////////////////////////////////////////////////////////
	double** featurevector;
	featurevector = new double*[2*ncor];
	for(k = 0; k < ncor; k++){
		featurevector[2*k] = new double[7];
		featurevector[2*k+1] = new double[7];
		sx1 = correspondantsA[k][0];
		sy1 = correspondantsA[k][1];
		sx2 = correspondantsB[k][0];
		sy2 = correspondantsB[k][1];
		featurevector[2*k][0] = sx1;
		featurevector[2*k][1] = sy1;
		featurevector[2*k][2] = 1;
		featurevector[2*k][3] = 0;
		featurevector[2*k][4] = 0;
		featurevector[2*k][5] = 0;
		featurevector[2*k][6] = -sx2;

		featurevector[2*k+1][0] = 0;
		featurevector[2*k+1][1] = 0;
		featurevector[2*k+1][2] = 0;
		featurevector[2*k+1][3] = sx1;
		featurevector[2*k+1][4] = sy1;
		featurevector[2*k+1][5] = 1;
		featurevector[2*k+1][6] = -sy2;
	}

/////////////////////////////////////////////////////////////
// Least square distance method
////////////////////////////////////////////////////////////
	double* w = new double[7];
	double** v = new double*[7];
	for(k = 0; k < 7; k++) v[k] = new double[7];

	int minwindex;
	double minw;

	svdcmp(featurevector, 2*ncor, 7, w, v);

	minwindex = 0;
	minw = w[0];
	for(k = 1; k < 7; k++){
		if(minw > w[k]){	minw = w[k];	minwindex = k;	}
	}

	Hmatrix[0][0] = (float)(v[0][minwindex] / v[6][minwindex]);
	Hmatrix[0][1] = (float)(v[1][minwindex] / v[6][minwindex]);
	Hmatrix[0][2] = (float)(v[2][minwindex] / v[6][minwindex]);
	Hmatrix[1][0] = (float)(v[3][minwindex] / v[6][minwindex]);
	Hmatrix[1][1] = (float)(v[4][minwindex] / v[6][minwindex]);
	Hmatrix[1][2] = (float)(v[5][minwindex] / v[6][minwindex]);
	Hmatrix[2][0] = 0.0f;
	Hmatrix[2][1] = 0.0f;
	Hmatrix[2][2] = 1.0f;

	for(k = 0; k < 2*ncor; k++) delete [] featurevector[k];
	delete [] featurevector;
	for(k = 0; k < 7; k++) delete [] v[k];
	delete [] v;
	delete [] w;
}

void Homography::ComputeRTHomography(double correspondants[4][4]){
	int k;
	double sx1,sy1,sx2,sy2;

/////////////////////////////////////////////////////////////
// Map correspondants to feature
////////////////////////////////////////////////////////////
	double** featurevector;
	featurevector = new double*[8];
	for(k = 0; k < 4; k++){
		featurevector[2*k] = new double[5];
		featurevector[2*k+1] = new double[5];
		sx1 = correspondants[k][0];
		sy1 = correspondants[k][1];
		sx2 = correspondants[k][2];
		sy2 = correspondants[k][3];
		featurevector[2*k][0] = sx1;
		featurevector[2*k][1] = sy1;
		featurevector[2*k][2] = 1;
		featurevector[2*k][3] = 0;
		featurevector[2*k][4] = -sx2;
		
		featurevector[2*k+1][0] = sy1;
		featurevector[2*k+1][1] = sx1;
		featurevector[2*k+1][2] = 0;
		featurevector[2*k+1][3] = 1;
		featurevector[2*k+1][4] = -sy2;
	}

/////////////////////////////////////////////////////////////
// Least square distance method
////////////////////////////////////////////////////////////
	double* w = new double[5];
	double** v = new double*[5];
	for(k = 0; k < 5; k++) v[k] = new double[5];

	int minwindex;
	double minw;

	svdcmp(featurevector, 8, 5, w, v);

	minwindex = 0;
	minw = w[0];
	for(k = 1; k < 5; k++){
		if(minw > w[k]){	minw = w[k];	minwindex = k;	}
	}

	Hmatrix[0][0] = (float)(v[0][minwindex] / v[4][minwindex]);
	Hmatrix[0][1] = (float)(v[1][minwindex] / v[4][minwindex]);
	Hmatrix[0][2] = (float)(v[2][minwindex] / v[4][minwindex]);
	Hmatrix[1][0] = (float)(v[1][minwindex] / v[4][minwindex]);
	Hmatrix[1][1] = (float)(v[0][minwindex] / v[4][minwindex]);
	Hmatrix[1][2] = (float)(v[3][minwindex] / v[4][minwindex]);
	Hmatrix[2][0] = 0.0f;
	Hmatrix[2][1] = 0.0f;
	Hmatrix[2][2] = 1.0f;

	for(k = 0; k < 8; k++) delete [] featurevector[k];
	delete [] featurevector;
	for(k = 0; k < 5; k++) delete [] v[k];
	delete [] v;
	delete [] w;
}

void Homography::ComputeRTHomography(double correspondantsA[4][2], double correspondantsB[4][2]){
	int k;
	double sx1,sy1,sx2,sy2;

/////////////////////////////////////////////////////////////
// Map correspondants to feature
////////////////////////////////////////////////////////////
	double** featurevector;
	featurevector = new double*[8];
	for(k = 0; k < 4; k++){
		featurevector[2*k] = new double[5];
		featurevector[2*k+1] = new double[5];
		sx1 = correspondantsA[k][0];
		sy1 = correspondantsA[k][1];
		sx2 = correspondantsB[k][0];
		sy2 = correspondantsB[k][1];
		featurevector[2*k][0] = sx1;
		featurevector[2*k][1] = sy1;
		featurevector[2*k][2] = 1;
		featurevector[2*k][3] = 0;
		featurevector[2*k][4] = -sx2;
		
		featurevector[2*k+1][0] = sy1;
		featurevector[2*k+1][1] = sx1;
		featurevector[2*k+1][2] = 0;
		featurevector[2*k+1][3] = 1;
		featurevector[2*k+1][4] = -sy2;
	}

/////////////////////////////////////////////////////////////
// Least square distance method
////////////////////////////////////////////////////////////
	double* w = new double[5];
	double** v = new double*[5];
	for(k = 0; k < 5; k++) v[k] = new double[5];

	int minwindex;
	double minw;

	svdcmp(featurevector, 8, 5, w, v);

	minwindex = 0;
	minw = w[0];
	for(k = 1; k < 5; k++){
		if(minw > w[k]){	minw = w[k];	minwindex = k;	}
	}

	Hmatrix[0][0] = (float)(v[0][minwindex] / v[4][minwindex]);
	Hmatrix[0][1] = (float)(v[1][minwindex] / v[4][minwindex]);
	Hmatrix[0][2] = (float)(v[2][minwindex] / v[4][minwindex]);
	Hmatrix[1][0] = (float)(v[1][minwindex] / v[4][minwindex]);
	Hmatrix[1][1] = (float)(v[0][minwindex] / v[4][minwindex]);
	Hmatrix[1][2] = (float)(v[3][minwindex] / v[4][minwindex]);
	Hmatrix[2][0] = 0.0f;
	Hmatrix[2][1] = 0.0f;
	Hmatrix[2][2] = 1.0f;

	for(k = 0; k < 8; k++) delete [] featurevector[k];
	delete [] featurevector;
	for(k = 0; k < 5; k++) delete [] v[k];
	delete [] v;
	delete [] w;
}

void Homography::ComputeRTHomography(double** correspondants, int ncor){
	int k;
	double sx1,sy1,sx2,sy2;

/////////////////////////////////////////////////////////////
// Map correspondants to feature
////////////////////////////////////////////////////////////
	double** featurevector;
	featurevector = new double*[2*ncor];
	for(k = 0; k < ncor; k++){
		featurevector[2*k] = new double[5];
		featurevector[2*k+1] = new double[5];
		sx1 = correspondants[k][0];
		sy1 = correspondants[k][1];
		sx2 = correspondants[k][2];
		sy2 = correspondants[k][3];
		featurevector[2*k][0] = sx1;
		featurevector[2*k][1] = sy1;
		featurevector[2*k][2] = 1;
		featurevector[2*k][3] = 0;
		featurevector[2*k][4] = -sx2;
		
		featurevector[2*k+1][0] = sy1;
		featurevector[2*k+1][1] = sx1;
		featurevector[2*k+1][2] = 0;
		featurevector[2*k+1][3] = 1;
		featurevector[2*k+1][4] = -sy2;
	}

/////////////////////////////////////////////////////////////
// Least square distance method
////////////////////////////////////////////////////////////
	double* w = new double[5];
	double** v = new double*[5];
	for(k = 0; k < 5; k++) v[k] = new double[5];

	int minwindex;
	double minw;

	svdcmp(featurevector, 2*ncor, 5, w, v);

	minwindex = 0;
	minw = w[0];
	for(k = 1; k < 5; k++){
		if(minw > w[k]){	minw = w[k];	minwindex = k;	}
	}

	Hmatrix[0][0] = (float)(v[0][minwindex] / v[4][minwindex]);
	Hmatrix[0][1] = (float)(v[1][minwindex] / v[4][minwindex]);
	Hmatrix[0][2] = (float)(v[2][minwindex] / v[4][minwindex]);
	Hmatrix[1][0] = (float)(v[1][minwindex] / v[4][minwindex]);
	Hmatrix[1][1] = (float)(v[0][minwindex] / v[4][minwindex]);
	Hmatrix[1][2] = (float)(v[3][minwindex] / v[4][minwindex]);
	Hmatrix[2][0] = 0.0f;
	Hmatrix[2][1] = 0.0f;
	Hmatrix[2][2] = 1.0f;

	for(k = 0; k < 2*ncor; k++) delete [] featurevector[k];
	delete [] featurevector;
	for(k = 0; k < 5; k++) delete [] v[k];
	delete [] v;
	delete [] w;
}

void Homography::ComputeRTHomography(double** correspondantsA, double** correspondantsB, int ncor){
	int k;
	double sx1,sy1,sx2,sy2;

/////////////////////////////////////////////////////////////
// Map correspondants to feature
////////////////////////////////////////////////////////////
	double** featurevector;
	featurevector = new double*[2*ncor];
	for(k = 0; k < ncor; k++){
		featurevector[2*k] = new double[5];
		featurevector[2*k+1] = new double[5];
		sx1 = correspondantsA[k][0];
		sy1 = correspondantsA[k][1];
		sx2 = correspondantsB[k][0];
		sy2 = correspondantsB[k][1];
		featurevector[2*k][0] = sx1;
		featurevector[2*k][1] = sy1;
		featurevector[2*k][2] = 1;
		featurevector[2*k][3] = 0;
		featurevector[2*k][4] = -sx2;
		
		featurevector[2*k+1][0] = sy1;
		featurevector[2*k+1][1] = sx1;
		featurevector[2*k+1][2] = 0;
		featurevector[2*k+1][3] = 1;
		featurevector[2*k+1][4] = -sy2;
	}

/////////////////////////////////////////////////////////////
// Least square distance method
////////////////////////////////////////////////////////////
	double* w = new double[5];
	double** v = new double*[5];
	for(k = 0; k < 5; k++) v[k] = new double[5];

	int minwindex;
	double minw;

	svdcmp(featurevector, 2*ncor, 5, w, v);

	minwindex = 0;
	minw = w[0];
	for(k = 1; k < 5; k++){
		if(minw > w[k]){	minw = w[k];	minwindex = k;	}
	}

	Hmatrix[0][0] = (float)(v[0][minwindex] / v[4][minwindex]);
	Hmatrix[0][1] = (float)(v[1][minwindex] / v[4][minwindex]);
	Hmatrix[0][2] = (float)(v[2][minwindex] / v[4][minwindex]);
	Hmatrix[1][0] = (float)(v[1][minwindex] / v[4][minwindex]);
	Hmatrix[1][1] = (float)(v[0][minwindex] / v[4][minwindex]);
	Hmatrix[1][2] = (float)(v[3][minwindex] / v[4][minwindex]);
	Hmatrix[2][0] = 0.0f;
	Hmatrix[2][1] = 0.0f;
	Hmatrix[2][2] = 1.0f;

	for(k = 0; k < 2*ncor; k++) delete [] featurevector[k];
	delete [] featurevector;
	for(k = 0; k < 5; k++) delete [] v[k];
	delete [] v;
	delete [] w;
}