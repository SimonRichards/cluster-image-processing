#ifndef __HOMOGRAPHY_H__
#define __HOMOGRAPHY_H__

class Homography{
public:
	Homography(){};
	~Homography(){};

	//From B to A
	void ComputeHomography(double correspondants[4][4]);
	void ComputeHomography(double correspondantsA[4][2], double correspondantsB[4][2]);
	void ComputeHomography(double correspondantsA[4][2], double correspondantsB[4][2], double** featurevector, double* w, double** v, double* rv1);
	void ComputeHomography(double** correspondants, int ncor);
	void ComputeHomography(double** correspondantsA, double** correspondantsB, int ncor);

	void ComputeAffineHomography(double correspondants[4][4]);
	void ComputeAffineHomography(double correspondantsA[4][2], double correspondantsB[4][2]);
	void ComputeAffineHomography(double** correspondants, int ncor);
	void ComputeAffineHomography(double** correspondantsA, double** correspondantsB, int ncor);

	void ComputeRTHomography(double correspondants[4][4]);
	void ComputeRTHomography(double correspondantsA[4][2], double correspondantsB[4][2]);
	void ComputeRTHomography(double** correspondants, int ncor);
	void ComputeRTHomography(double** correspondantsA, double** correspondantsB, int ncor);

	void MatrixInverse(float A[3][3], float I[3][3]){
		double detA = A[0][0]*A[1][1]*A[2][2] + A[1][0]*A[2][1]*A[0][2] + A[0][1]*A[1][2]*A[2][0] - 
					A[0][0]*A[1][2]*A[2][1] - A[0][1]*A[1][0]*A[2][2] - A[0][2]*A[2][0]*A[1][1] ;

		I[0][0] = (float)((A[1][1]*A[2][2] - A[1][2]*A[2][1]) / detA);
		I[0][1] = (float)((A[0][2]*A[2][1] - A[0][1]*A[2][2]) / detA);
		I[0][2] = (float)((A[0][1]*A[1][2] - A[0][2]*A[1][1]) / detA);
		I[1][0] = (float)((A[1][2]*A[2][0] - A[1][0]*A[2][2]) / detA);
		I[1][1] = (float)((A[0][0]*A[2][2] - A[0][2]*A[2][0]) / detA);
		I[1][2] = (float)((A[0][2]*A[1][0] - A[0][0]*A[1][2]) / detA);
		I[2][0] = (float)((A[1][0]*A[2][1] - A[1][1]*A[2][0]) / detA);
		I[2][1] = (float)((A[0][1]*A[2][0] - A[0][0]*A[2][1]) / detA);
		I[2][2] = (float)((A[0][0]*A[1][1] - A[0][1]*A[1][0]) / detA);
	}

	inline void Transform(float& x, float& y){
		float z = Hmatrix[2][0] * x + Hmatrix[2][1] * y + Hmatrix[2][2];
		float fx = (Hmatrix[0][0] * x + Hmatrix[0][1] * y + Hmatrix[0][2])/z;
		float fy = (Hmatrix[1][0] * x + Hmatrix[1][1] * y + Hmatrix[1][2])/z;
		x = fx;
		y = fy;
	}

	/////////
	//  A[0]         H[0][0] H[0][1] H[0][2]      B[0]
	//[ A[1] ] =  [  H[1][0] H[1][1] H[1][2]  ] [ B[1] ]
	//  A[2]         H[2][0] H[2][1] H[2][2]      B[2]
	/////////

	float Hmatrix[3][3];
};

#endif