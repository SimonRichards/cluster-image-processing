#ifndef __PROJECTIVE_MOTION_RL_H__
#define __PROJECTIVE_MOTION_RL_H__

#include "Homography.h"
#include <memory>
#include <cmath>
#include <cstring>
#include <cstdlib>

#ifndef PI
#define PI 3.141592653589793238462643383279502884197169399375105820974944592f
#endif

//#define __SHOWERROR__

#define NumSamples 30

class ProjectiveMotionRL{
public:
	ProjectiveMotionRL(){
		int i;
		for(i = 0; i < NumSamples; i++){
			Hmatrix[i].Hmatrix[0][0] = 1;	Hmatrix[i].Hmatrix[0][1] = 0;	Hmatrix[i].Hmatrix[0][2] =0;
			Hmatrix[i].Hmatrix[1][0] = 0;	Hmatrix[i].Hmatrix[1][1] = 1;	Hmatrix[i].Hmatrix[1][2] =0;
			Hmatrix[i].Hmatrix[2][0] = 0;	Hmatrix[i].Hmatrix[2][1] = 0;	Hmatrix[i].Hmatrix[2][2] =1;
			IHmatrix[i].Hmatrix[0][0] = 1;	IHmatrix[i].Hmatrix[0][1] = 0;	IHmatrix[i].Hmatrix[0][2] =0;
			IHmatrix[i].Hmatrix[1][0] = 0;	IHmatrix[i].Hmatrix[1][1] = 1;	IHmatrix[i].Hmatrix[1][2] =0;
			IHmatrix[i].Hmatrix[2][0] = 0;	IHmatrix[i].Hmatrix[2][1] = 0;	IHmatrix[i].Hmatrix[2][2] =1;
		}
		WarpImgBuffer = NULL;
		WarpImgBufferR = NULL;
		WarpImgBufferG = NULL;
		WarpImgBufferB = NULL;
		WarpWeightBuffer = NULL;
		BlurImgBuffer = NULL;
		BlurImgBufferR = NULL;
		BlurImgBufferG = NULL;
		BlurImgBufferB = NULL;
		BlurWeightBuffer = NULL;
		ErrorImgBuffer = NULL;
		ErrorImgBufferR = NULL;
		ErrorImgBufferG = NULL;
		ErrorImgBufferB = NULL;
		ErrorWeightBuffer = NULL;

		SetSpsTable();
		SetBilateralTable();

		GroundTruthImg = NULL;
		GroundTruthImgR = NULL;
		GroundTruthImgG = NULL;
		GroundTruthImgB = NULL;
	}
	~ProjectiveMotionRL(){
		ClearBuffer();
		if( GroundTruthImg ){
			delete [] GroundTruthImg;
			GroundTruthImg = NULL;
		}
		if( GroundTruthImgR ){
			delete [] GroundTruthImgR;
			GroundTruthImgR = NULL;
		}
		if( GroundTruthImgG ){
			delete [] GroundTruthImgG;
			GroundTruthImgG = NULL;
		}
		if( GroundTruthImgB ){
			delete [] GroundTruthImgB;
			GroundTruthImgB = NULL;
		}
	}
	
	////////////////////////////////////
	//These functions are used to set Buffer for caching
	////////////////////////////////////
	void SetBuffer(int width, int height){
		WarpImgBuffer = new float[width*height];
		WarpImgBufferR = new float[width*height];
		WarpImgBufferG = new float[width*height];
		WarpImgBufferB = new float[width*height];
		WarpWeightBuffer = new float[width*height];
		BlurImgBuffer = new float[width*height];
		BlurImgBufferR = new float[width*height];
		BlurImgBufferG = new float[width*height];
		BlurImgBufferB = new float[width*height];
		BlurWeightBuffer = new float[width*height];
		ErrorImgBuffer = new float[width*height];
		ErrorImgBufferR = new float[width*height];
		ErrorImgBufferG = new float[width*height];
		ErrorImgBufferB = new float[width*height];
		ErrorWeightBuffer = new float[width*height];
	}
	void ClearBuffer(){
		if(WarpImgBuffer){
			delete [] WarpImgBuffer;
			WarpImgBuffer = NULL;
		}
		if(WarpImgBufferR){
			delete [] WarpImgBufferR;
			WarpImgBufferR = NULL;
		}
		if(WarpImgBufferG){
			delete [] WarpImgBufferG;
			WarpImgBufferG = NULL;
		}
		if(WarpImgBufferB){
			delete [] WarpImgBufferB;
			WarpImgBufferB = NULL;
		}
		if(WarpWeightBuffer){
			delete [] WarpWeightBuffer;
			WarpWeightBuffer = NULL;
		}
		if(BlurImgBuffer){
			delete [] BlurImgBuffer;
			BlurImgBuffer = NULL;
		}
		if(BlurImgBufferR){
			delete [] BlurImgBufferR;
			BlurImgBufferR = NULL;
		}
		if(BlurImgBufferG){
			delete [] BlurImgBufferG;
			BlurImgBufferG = NULL;
		}
		if(BlurImgBufferB){
			delete [] BlurImgBufferB;
			BlurImgBufferB = NULL;
		}
		if(BlurWeightBuffer){
			delete [] BlurWeightBuffer;
			BlurWeightBuffer = NULL;
		}
		if(ErrorImgBuffer){
			delete [] ErrorImgBuffer;
			ErrorImgBuffer = NULL;
		}
		if(ErrorImgBufferR){
			delete [] ErrorImgBufferR;
			ErrorImgBufferR = NULL;
		}
		if(ErrorImgBufferG){
			delete [] ErrorImgBufferG;
			ErrorImgBufferG = NULL;
		}
		if(ErrorImgBufferB){
			delete [] ErrorImgBufferB;
			ErrorImgBufferB = NULL;
		}
		if(ErrorWeightBuffer){
			delete [] ErrorWeightBuffer;
			ErrorWeightBuffer = NULL;
		}
	}

	void SetSpsTable(){
		int i, t = 1;
		//Parameters, e.g. noiseVar and epilson, are set according to Levin et al Siggraph'07
		float powD = 0.8f, noiseVar = 0.005f, epilson = t / 255.0f;
		float minWeight = exp(-pow(epilson,powD) / noiseVar) * pow(epilson,powD-1.0f), minEpi = pow(epilson,powD-2.0f);

		for(i = 0; i <= t; i++){
			SpsTable[i] = 1.0f;
		}
		for(i = t+1; i < 256; i++){
			SpsTable[i] = (exp(-pow(i/255.0f,powD) / noiseVar) * pow(i/255.0f,powD-1.0f) ) / minWeight ;
			
			//Reweighting scheme from Levin et al Siggraph'07, 
			//Similar Effect, smaller smoothing weight for large gradient
			//SpsTable[i] = pow(i/255.0f,powD-2.0f) / minEpi ;
		}
	}

	void SetBilateralTable(){
		int i, t = 1;
		//Parameters are set according to Levin et al Siggraph'07
		//Better result can be obtained by using smaller noiseVar, but for fairness, we use the same setting.
		float powD = 0.8f, noiseVar = 0.005f, epilson = t / 255.0f;
		float minWeight = exp(-pow(epilson,powD) / noiseVar) * pow(epilson,powD-1.0f);

		//Standard Bilateral Weight
		for(i = 0; i < 256; i++){
			BilateralTable[i] = exp(-i*i / (noiseVar * 65025.0f) ) ;
		}
		
		//Bilateral Laplician Regularization
	//	for(i = 0; i <= t; i++){
	//		BilateralTable[i] = 1.0f;
	//	}
	//	for(i = t+1; i < 256; i++){
	//		BilateralTable[i] = (exp(-pow(i/255.0f,powD) / noiseVar) * pow(i/255.0f,powD-1.0f) ) / minWeight;
	//	}
	}	
	void SetGroundTruthImg(float* GroundTruth, int width, int height){
		if( GroundTruthImg ){
			delete [] GroundTruthImg;
			GroundTruthImg = new float[width*height];
		}
		memcpy(GroundTruthImg, GroundTruth, width*height*sizeof(float));
	}
	void SetGroundTruthImg(float* GroundTruthR, float* GroundTruthG, float* GroundTruthB, int width, int height){
		if( GroundTruthImgR ){
			delete [] GroundTruthImgR;
		}
		GroundTruthImgR = new float[width*height];
		if( GroundTruthImgG ){
			delete [] GroundTruthImgG;
		}
		GroundTruthImgG = new float[width*height];
		if( GroundTruthImgB ){
			delete [] GroundTruthImgB;
		}
		GroundTruthImgB = new float[width*height];
		memcpy(GroundTruthImgR, GroundTruthR, width*height*sizeof(float));
		memcpy(GroundTruthImgG, GroundTruthG, width*height*sizeof(float));
		memcpy(GroundTruthImgB, GroundTruthB, width*height*sizeof(float));
	}
	
	////////////////////////////////////
	//These functions are used to set the homography
	////////////////////////////////////
	void SetHomography(Homography H, int i){
		if(i >= 0 && i < NumSamples){
			memcpy(Hmatrix[i].Hmatrix[0], H.Hmatrix[0], 3*sizeof(float));
			memcpy(Hmatrix[i].Hmatrix[1], H.Hmatrix[1], 3*sizeof(float));
			memcpy(Hmatrix[i].Hmatrix[2], H.Hmatrix[2], 3*sizeof(float));

			Hmatrix[i].MatrixInverse(Hmatrix[i].Hmatrix, IHmatrix[i].Hmatrix);
		}
	}

	void SetGlobalRotation(float degree){
		int i;
		float deltadegree = (degree * PI / 180.0f) / NumSamples;
		for(i = 0; i < NumSamples; i++){
			Hmatrix[i].Hmatrix[0][0] = cos(deltadegree*i);	Hmatrix[i].Hmatrix[0][1] = sin(deltadegree*i);	Hmatrix[i].Hmatrix[0][2] =0;
			Hmatrix[i].Hmatrix[1][0] = -sin(deltadegree*i);	Hmatrix[i].Hmatrix[1][1] = cos(deltadegree*i);	Hmatrix[i].Hmatrix[1][2] =0;
			Hmatrix[i].Hmatrix[2][0] = 0;	Hmatrix[i].Hmatrix[2][1] = 0;	Hmatrix[i].Hmatrix[2][2] =1;
			IHmatrix[i].Hmatrix[0][0] = cos(deltadegree*i);	IHmatrix[i].Hmatrix[0][1] = -sin(deltadegree*i);	IHmatrix[i].Hmatrix[0][2] =0;
			IHmatrix[i].Hmatrix[1][0] = sin(deltadegree*i);	IHmatrix[i].Hmatrix[1][1] = cos(deltadegree*i);	IHmatrix[i].Hmatrix[1][2] =0;
			IHmatrix[i].Hmatrix[2][0] = 0;	IHmatrix[i].Hmatrix[2][1] = 0;	IHmatrix[i].Hmatrix[2][2] =1;
		}
	}
	void SetGlobalScaling(float scalefactor){
		int i;
		float deltascale = (scalefactor-1.0f) / NumSamples;
		for(i = 0; i < NumSamples; i++){
			Hmatrix[i].Hmatrix[0][0] = 1.0f+i*deltascale;	Hmatrix[i].Hmatrix[0][1] = 0;	Hmatrix[i].Hmatrix[0][2] =0;
			Hmatrix[i].Hmatrix[1][0] = 0;	Hmatrix[i].Hmatrix[1][1] = 1.0f+i*deltascale;	Hmatrix[i].Hmatrix[1][2] =0;
			Hmatrix[i].Hmatrix[2][0] = 0;	Hmatrix[i].Hmatrix[2][1] = 0;	Hmatrix[i].Hmatrix[2][2] =1;
			IHmatrix[i].Hmatrix[0][0] = 1.0f / (1.0f+i*deltascale);	IHmatrix[i].Hmatrix[0][1] = 0;	IHmatrix[i].Hmatrix[0][2] =0;
			IHmatrix[i].Hmatrix[1][0] = 0;	IHmatrix[i].Hmatrix[1][1] = 1.0f / (1.0f+i*deltascale);	IHmatrix[i].Hmatrix[1][2] =0;
			IHmatrix[i].Hmatrix[2][0] = 0;	IHmatrix[i].Hmatrix[2][1] = 0;	IHmatrix[i].Hmatrix[2][2] =1;
		}
	}
	void SetGlobalTranslation(float dx, float dy){
		int i;
		float deltadx = dx / NumSamples;
		float deltady = dy / NumSamples;
		for(i = 0; i < NumSamples; i++){
			Hmatrix[i].Hmatrix[0][0] = 1;	Hmatrix[i].Hmatrix[0][1] = 0;	Hmatrix[i].Hmatrix[0][2] =i*deltadx;
			Hmatrix[i].Hmatrix[1][0] = 0;	Hmatrix[i].Hmatrix[1][1] = 1;	Hmatrix[i].Hmatrix[1][2] =i*deltady;
			Hmatrix[i].Hmatrix[2][0] = 0;	Hmatrix[i].Hmatrix[2][1] = 0;	Hmatrix[i].Hmatrix[2][2] =1;
			IHmatrix[i].Hmatrix[0][0] = 1;	IHmatrix[i].Hmatrix[0][1] = 0;	IHmatrix[i].Hmatrix[0][2] =-i*deltadx;
			IHmatrix[i].Hmatrix[1][0] = 0;	IHmatrix[i].Hmatrix[1][1] = 1;	IHmatrix[i].Hmatrix[1][2] =-i*deltady;
			IHmatrix[i].Hmatrix[2][0] = 0;	IHmatrix[i].Hmatrix[2][1] = 0;	IHmatrix[i].Hmatrix[2][2] =1;
		}
	}
	void SetGlobalPerspective(float px, float py){
		int i;
		float deltapx = px / NumSamples;
		float deltapy = py / NumSamples;
		for(i = 0; i < NumSamples; i++){
			Hmatrix[i].Hmatrix[0][0] = 1;	Hmatrix[i].Hmatrix[0][1] = 0;	Hmatrix[i].Hmatrix[0][2] =0;
			Hmatrix[i].Hmatrix[1][0] = 0;	Hmatrix[i].Hmatrix[1][1] = 1;	Hmatrix[i].Hmatrix[1][2] =0;
			Hmatrix[i].Hmatrix[2][0] = i*deltapx;	Hmatrix[i].Hmatrix[2][1] = i*deltapy;	Hmatrix[i].Hmatrix[2][2] =1;
			IHmatrix[i].Hmatrix[0][0] = 1;	IHmatrix[i].Hmatrix[0][1] = 0;	IHmatrix[i].Hmatrix[0][2] =0;
			IHmatrix[i].Hmatrix[1][0] = 0;	IHmatrix[i].Hmatrix[1][1] = 1;	IHmatrix[i].Hmatrix[1][2] =0;
			IHmatrix[i].Hmatrix[2][0] = -i*deltapx;	IHmatrix[i].Hmatrix[2][1] = -i*deltapy;	IHmatrix[i].Hmatrix[2][2] =1;
		}
	}

	void SetGlobalParameters(float degree, float scalefactor, float px, float py, float dx, float dy){
		int i;
		float deltadegree = (degree * PI / 180.0f) / NumSamples;
		float deltascale = (scalefactor-1.0f) / NumSamples;
		float deltapx = px / NumSamples;
		float deltapy = py / NumSamples;
		float deltadx = dx / NumSamples;
		float deltady = dy / NumSamples;
		for(i = 0; i < NumSamples; i++){
			Hmatrix[i].Hmatrix[0][0] = cos(deltadegree*i)*(1.0f+i*deltascale);	Hmatrix[i].Hmatrix[0][1] = sin(deltadegree*i);	Hmatrix[i].Hmatrix[0][2] =i*deltadx;
			Hmatrix[i].Hmatrix[1][0] = -sin(deltadegree*i);	Hmatrix[i].Hmatrix[1][1] = cos(deltadegree*i)*(1.0f+i*deltascale);	Hmatrix[i].Hmatrix[1][2] =i*deltady;
			Hmatrix[i].Hmatrix[2][0] = i*deltapx;	Hmatrix[i].Hmatrix[2][1] = i*deltapy;	Hmatrix[i].Hmatrix[2][2] =1;
			
			Hmatrix[i].MatrixInverse(Hmatrix[i].Hmatrix, IHmatrix[i].Hmatrix);
		}
	}

	
	////////////////////////////////////
	//These functions are used to generate the Projective Motion Blur Images
	////////////////////////////////////
	//i: postive forward, negative backward
	void WarpImage(float* InputImg, float* inputWeight, int iwidth, int iheight, float* OutputImg, float* outputWeight, int width, int height, int i);
	void WarpImage(float* InputImgR, float* InputImgG, float* InputImgB, float* inputWeight, int iwidth, int iheight, float* OutputImgR, float* OutputImgG, float* OutputImgB, float* outputWeight, int width, int height, int i);
	//bforward: true forward, false backward
	void GenerateMotionBlurImg(float* InputImg, float* inputWeight, int iwidth, int iheight, float* BlurImg, float* outputWeight, int width, int height, bool bforward = true);
	void GenerateMotionBlurImg(float* InputImgR, float* InputImgG, float* InputImgB, float* inputWeight, int iwidth, int iheight, float* BlurImgR, float* BlurImgG, float* BlurImgB, float* outputWeight, int width, int height, bool bforward = true);
	
	////////////////////////////////////
	//These functions are deblurring algorithm
	////////////////////////////////////
	//This is the Basic algorithm
	//DeblurImg: the Input itself is initialization, so you can load yBilateralLap own initialization
	void ProjectiveMotionRLDeblur(float* BlurImg, int iwidth, int iheight, float* DeblurImg, int width, int height, int Niter = 20, bool bPoisson = true);
	void ProjectiveMotionRLDeblur(float* BlurImgR, float* BlurImgG, float* BlurImgB, int iwidth, int iheight, float* DeblurImgR, float* DeblurImgG, float* DeblurImgB, int width, int height, int Niter = 20, bool bPoisson = true);
	
	//This are the deblurring algorithm with regularization
	//Details please refers to paper
	//The lambda in TV regularization is 0.002, but it's un-normalized weight
	//Intensity range is between 0 and 1, so, the actual weight is 0.002f * 255 = 0.51f for normalized weight
	void ProjectiveMotionRLDeblurTVReg(float* BlurImg, int iwidth, int iheight, float* DeblurImg, int width, int height, int Niter = 20, bool bPoisson = true, float lambda = 0.50f);
	void ProjectiveMotionRLDeblurTVReg(float* BlurImgR, float* BlurImgG, float* BlurImgB, int iwidth, int iheight, float* DeblurImgR, float* DeblurImgG, float* DeblurImgB, int width, int height, int Niter = 20, bool bPoisson = true, float lambda = 0.50f);
	//Value of lambda used in Levin et al is also un-normalized by minWeight, hence it's much smaller
	//The typical range of this Sps in their implementation is between 0.001 - 0.004
	//We use the same lambda as in TV regularization for better comparison
	void ProjectiveMotionRLDeblurSpsReg(float* BlurImg, int iwidth, int iheight, float* DeblurImg, int width, int height, int Niter = 20, bool bPoisson = true, float lambda = 0.50f);
	void ProjectiveMotionRLDeblurSpsReg(float* BlurImgR, float* BlurImgG, float* BlurImgB, int iwidth, int iheight, float* DeblurImgR, float* DeblurImgG, float* DeblurImgB, int width, int height, int Niter = 20, bool bPoisson = true, float lambda = 0.50f);
	//We use the same lambda as in TV regularization for better comparison
	//Parameter setting, noise variance, for bilateral reg is the same as laplacian reg.
	void ProjectiveMotionRLDeblurBilateralReg(float* BlurImg, int iwidth, int iheight, float* DeblurImg, int width, int height, int Niter = 20, bool bPoisson = true, float lambda = 0.50f);
	void ProjectiveMotionRLDeblurBilateralReg(float* BlurImgR, float* BlurImgG, float* BlurImgB, int iwidth, int iheight, float* DeblurImgR, float* DeblurImgG, float* DeblurImgB, int width, int height, int Niter = 20, bool bPoisson = true, float lambda = 0.50f);
	//This is the bilateral laplacian regularization
	void ProjectiveMotionRLDeblurBilateralLapReg(float* BlurImg, int iwidth, int iheight, float* DeblurImg, int width, int height, int Niter = 20, bool bPoisson = true, float lambda = 0.50f);
	void ProjectiveMotionRLDeblurBilateralLapReg(float* BlurImgR, float* BlurImgG, float* BlurImgB, int iwidth, int iheight, float* DeblurImgR, float* DeblurImgG, float* DeblurImgB, int width, int height, int Niter = 20, bool bPoisson = true, float lambda = 0.50f);

	////////////////////////////////////
	//These functions are used to compute derivatives for regularization
	////////////////////////////////////
	void ComputeGradientXImage(float* Img, int width, int height, float* DxImg, bool bflag = true);
	void ComputeGradientYImage(float* Img, int width, int height, float* DyImg, bool bflag = true);
	void ComputeGradientImage(float* Img, int width, int height, float* DxImg, float* DyImg, bool bflag = true);
	void ComputeBilaterRegImage(float* Img, int width, int height, float* BRImg);

	////////////////////////////////////
	//These functions are used to compute Errors
	////////////////////////////////////
	float ComputeRMSError(float* GroundTruth, float* DeblurredImg, int width, int height){
		float RMS = 0;
		if( GroundTruth ){
			int x,y,index;
			for(y = 0, index = 0; y < height; y++){
				for(x = 0; x < width; x++, index++){
					RMS += (GroundTruth[index] - DeblurredImg[index])*(GroundTruth[index] - DeblurredImg[index]);
				}
			}
		}
		//The RMS error output in paper have been multiplied by 255
		return sqrt(RMS / (width*height));
	}

	////////////////////////////////////
	//These functions are used to generate noise
	////////////////////////////////////
	// Normal random number generator, variance = 1
	float normalrand() {
		float val = 0;
		for(int i = 0; i != 12; ++i)
			val += ((float)(rand()) / RAND_MAX);
		return val - 6.0f;
	}
	//Noise variance = amp
	void gaussianNoise(float* Img, int width, int height, float amp) {
		int x,y,index;
		float random,noise;
		for(y = 0, index = 0; y < height; y++) {
			for(x = 0; x < width; x++, index++) {
				random = normalrand() / 255.0f;
				noise = amp * random;
				Img[index] += noise;
				if(Img[index] > 1.0f) Img[index] = 1.0f;
				if(Img[index] < 0.0f) Img[index] = 0.0f;
			}
		}
	}
	
	//Multiscale version: not very useful...
	//void ProjectiveMotionRLDeblurMultiScale(float* BlurImg, int iwidth, int iheight, float* DeblurImg, int width, int height, int Niter = 10, int Nscale = 5, bool bPoisson = true);

//private:

	//These are the homography sequence for Projective motion blur model
	Homography Hmatrix[NumSamples];
	Homography IHmatrix[NumSamples];

	//These are buffer and lookup table variables
	float BilateralTable[256];
	float SpsTable[256];
	float* WarpImgBuffer;
	float* WarpImgBufferR;
	float* WarpImgBufferG;
	float* WarpImgBufferB;
	float* WarpWeightBuffer;
	float* BlurImgBuffer;
	float* BlurImgBufferR;
	float* BlurImgBufferG;
	float* BlurImgBufferB;
	float* BlurWeightBuffer;
	float* ErrorImgBuffer;
	float* ErrorImgBufferR;
	float* ErrorImgBufferG;
	float* ErrorImgBufferB;
	float* ErrorWeightBuffer;

	//This variables are used for RMS computation
	float* GroundTruthImg;
	float* GroundTruthImgR;
	float* GroundTruthImgG;
	float* GroundTruthImgB;
};

#endif
