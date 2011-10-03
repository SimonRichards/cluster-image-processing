#include "ProjectiveMotionRL.h"
#include "ImResize.h"
#include "bitmap.h"

#include <cmath>
#include <ctime>

int main(int argc, char* argv[]){
	srand(time(0));

	int width, height;
	float* fImg[3];
	char prefix[256] = "doll";
	//memcpy(prefix, argv[1], strlen(argv[1])+1);
	char fname[256];
	sprintf(fname, "%s.bmp", prefix);
	printf("Load Image: %s\n", fname);
	readBMP(fname, fImg[0], fImg[1], fImg[2], width, height);
	int blurwidth = width, blurheight = height;

	float* bImg[3];
	float* deblurImg[3];
	float* intermediatedeblurImg[3];
	float* inputWeight;
	float* outputWeight;
	float RMSError;
	bImg[0] = new float[width*height];
	bImg[1] = new float[width*height];
	bImg[2] = new float[width*height];
	inputWeight = NULL;
	outputWeight = new float[width*height];
	deblurImg[0] = new float[width*height];
	deblurImg[1] = new float[width*height];
	deblurImg[2] = new float[width*height];
	intermediatedeblurImg[0] = new float[width*height];
	intermediatedeblurImg[1] = new float[width*height];
	intermediatedeblurImg[2] = new float[width*height];

///////////////////////////////////	
	printf("Set Projective Model Parameter\n");
	ProjectiveMotionRL m_ProjectiveMotionRL;

	m_ProjectiveMotionRL.SetGlobalParameters(10, 1.2f, 0.0003f,0.0006f, 10, 20); //This is parameter setting for doll example
	//m_ProjectiveMotionRL.SetGlobalParameters(-10, 1.1f, 0.0004f,0.0002f, 20, -15); //This is parameter setting for Cameraman convergence example

	// Testing case for rotational motion
	//m_ProjectiveMotionRL.SetGlobalRotation(10);
	// Testing case for zooming motion
	//m_ProjectiveMotionRL.SetGlobalScaling(1.2f);
	// Testing case for translational motion
	/*float deltadx = 0.8f;
	for(int i = 0; i < NumSamples; i++){
		float dy = 5.0f * sin( (float)(i)/NumSamples * 2 * PI);
		m_ProjectiveMotionRL.Hmatrix[i].Hmatrix[0][0] = 1;	m_ProjectiveMotionRL.Hmatrix[i].Hmatrix[0][1] = 0;	m_ProjectiveMotionRL.Hmatrix[i].Hmatrix[0][2] =i*deltadx;
		m_ProjectiveMotionRL.Hmatrix[i].Hmatrix[1][0] = 0;	m_ProjectiveMotionRL.Hmatrix[i].Hmatrix[1][1] = 1;	m_ProjectiveMotionRL.Hmatrix[i].Hmatrix[1][2] =dy;
		m_ProjectiveMotionRL.Hmatrix[i].Hmatrix[2][0] = 0;	m_ProjectiveMotionRL.Hmatrix[i].Hmatrix[2][1] = 0;	m_ProjectiveMotionRL.Hmatrix[i].Hmatrix[2][2] =1;
		m_ProjectiveMotionRL.Hmatrix[i].MatrixInverse(m_ProjectiveMotionRL.Hmatrix[i].Hmatrix, m_ProjectiveMotionRL.IHmatrix[i].Hmatrix);
	}*/
	//Testing case for projective motion
	//m_ProjectiveMotionRL.SetGlobalPerspective(0.001f,0.001f);

	//Adjust the parameters for generating test case 1-15
	/*float deltadx = 0.8f;
	float deltascaling = 0.2f / NumSamples;
	float deltapx = 0.001f / NumSamples;
	float deltapy = 0.001f / NumSamples;
	float deltadegree = (10.0f * PI / 180.0f) / NumSamples;
	for(int i = 0; i < NumSamples; i++){
		float dy = 5.0f * sin( (float)(i)/NumSamples * 2 * PI);
		m_ProjectiveMotionRL.Hmatrix[i].Hmatrix[0][0] = (1+i*deltascaling)*cos(deltadegree*i);	m_ProjectiveMotionRL.Hmatrix[i].Hmatrix[0][1] = sin(deltadegree*i);	m_ProjectiveMotionRL.Hmatrix[i].Hmatrix[0][2] =i*deltadx;
		m_ProjectiveMotionRL.Hmatrix[i].Hmatrix[1][0] = -sin(deltadegree*i);	m_ProjectiveMotionRL.Hmatrix[i].Hmatrix[1][1] = (1+i*deltascaling)*cos(deltadegree*i);	m_ProjectiveMotionRL.Hmatrix[i].Hmatrix[1][2] =dy;
		m_ProjectiveMotionRL.Hmatrix[i].Hmatrix[2][0] = i*deltapx;	m_ProjectiveMotionRL.Hmatrix[i].Hmatrix[2][1] = i*deltapx;	m_ProjectiveMotionRL.Hmatrix[i].Hmatrix[2][2] =1;
		m_ProjectiveMotionRL.Hmatrix[i].MatrixInverse(m_ProjectiveMotionRL.Hmatrix[i].Hmatrix, m_ProjectiveMotionRL.IHmatrix[i].Hmatrix);
	}*/

///////////////////////////////////
	m_ProjectiveMotionRL.SetGroundTruthImg(fImg[0], fImg[1], fImg[2], width, height); //This is for error computation

///////////////////////////////////
	printf("Generate Motion Blurred Image\n");
	m_ProjectiveMotionRL.GenerateMotionBlurImg(fImg[0], inputWeight, width, height, bImg[0], outputWeight, blurwidth, blurheight, true);
	m_ProjectiveMotionRL.GenerateMotionBlurImg(fImg[1], inputWeight, width, height, bImg[1], outputWeight, blurwidth, blurheight, true);
	m_ProjectiveMotionRL.GenerateMotionBlurImg(fImg[2], inputWeight, width, height, bImg[2], outputWeight, blurwidth, blurheight, true);

	RMSError =	(m_ProjectiveMotionRL.ComputeRMSError(fImg[0], bImg[0], width, height) +
				m_ProjectiveMotionRL.ComputeRMSError(fImg[1], bImg[1], width, height) +
				m_ProjectiveMotionRL.ComputeRMSError(fImg[2], bImg[2], width, height))/3.0f;
	sprintf(fname, "%s_blur_%.6f.bmp", prefix,RMSError*255.0f);
	printf("Save Blurred Image to: %s\n", fname);
	writeBMP(fname, blurwidth, blurheight, bImg[0], bImg[1], bImg[2]);

	//Add noise
	float sigma = 2.0f;
	m_ProjectiveMotionRL.gaussianNoise(bImg[0], width, height, sigma);
	m_ProjectiveMotionRL.gaussianNoise(bImg[1], width, height, sigma);
	m_ProjectiveMotionRL.gaussianNoise(bImg[2], width, height, sigma);
	RMSError =	(m_ProjectiveMotionRL.ComputeRMSError(fImg[0], bImg[0], width, height) +
				m_ProjectiveMotionRL.ComputeRMSError(fImg[1], bImg[1], width, height) +
				m_ProjectiveMotionRL.ComputeRMSError(fImg[2], bImg[2], width, height))/3.0f;
	sprintf(fname, "%s_blur_noise_sigma%.2f_%.6f.bmp", prefix,sigma,RMSError*255.0f);
	printf("Save Blurred Image to: %s\n", fname);
	writeBMP(fname, blurwidth, blurheight, bImg[0], bImg[1], bImg[2]);

///////////////////////////////////
//Main Deblurring algorithm	
	printf("Initial Estimation is the blur image\n");
	ImChopping(bImg[0], blurwidth, blurheight, deblurImg[0], width, height);
	ImChopping(bImg[1], blurwidth, blurheight, deblurImg[1], width, height);
	ImChopping(bImg[2], blurwidth, blurheight, deblurImg[2], width, height);
//	memset(deblurImg[0], 0, width*height*sizeof(float));
//	memset(deblurImg[1], 0, width*height*sizeof(float));
//	memset(deblurImg[2], 0, width*height*sizeof(float));


	//Levin et. al. Siggraph07's matlab implementation also take around 400 iterations
	//Sadly, the algorithm needs such a lot of iterations to produce good results
	//Most running time were spent on bicubic interpolation, it would be much faster if this step was implemented in GPU...
	
	//Load Initial Guess, if you have...
	//readBMP("", deblurImg[0], deblurImg[1], deblurImg[2], width, height);
	memcpy(intermediatedeblurImg[0], deblurImg[0], width*height*sizeof(float));
	memcpy(intermediatedeblurImg[1], deblurImg[1], width*height*sizeof(float));
	memcpy(intermediatedeblurImg[2], deblurImg[2], width*height*sizeof(float));

	printf("Basic Algorithm:\n");
	memcpy(deblurImg[0], intermediatedeblurImg[0], width*height*sizeof(float));
	memcpy(deblurImg[1], intermediatedeblurImg[1], width*height*sizeof(float));
	memcpy(deblurImg[2], intermediatedeblurImg[2], width*height*sizeof(float));
	
	m_ProjectiveMotionRL.ProjectiveMotionRLDeblur(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 500, true);
	RMSError =	(m_ProjectiveMotionRL.ComputeRMSError(fImg[0], deblurImg[0], width, height) +
				m_ProjectiveMotionRL.ComputeRMSError(fImg[1], deblurImg[1], width, height) +
				m_ProjectiveMotionRL.ComputeRMSError(fImg[2], deblurImg[2], width, height))/3.0f;
	sprintf(fname, "%s_deblurBasic_%f.bmp", prefix, RMSError*255.0f);
	printf("Done, RMS Error: %f\n", RMSError*255.0f);
	writeBMP(fname, width, height, deblurImg[0], deblurImg[1], deblurImg[2]);

	printf("TV Regularization Algorithm:\n");
	memcpy(deblurImg[0], intermediatedeblurImg[0], width*height*sizeof(float));
	memcpy(deblurImg[1], intermediatedeblurImg[1], width*height*sizeof(float));
	memcpy(deblurImg[2], intermediatedeblurImg[2], width*height*sizeof(float));
	
	//Gradually decrease the regularization weight, otherwise, the result will be over smooth.
	//Actually, the following regularization produce similar results....
//	m_ProjectiveMotionRL.ProjectiveMotionRLDeblurTVReg(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 500, true, 0.5f);
	m_ProjectiveMotionRL.ProjectiveMotionRLDeblurTVReg(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 100, true, 1.0f);
	m_ProjectiveMotionRL.ProjectiveMotionRLDeblurTVReg(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 100, true, 0.5f);
	m_ProjectiveMotionRL.ProjectiveMotionRLDeblurTVReg(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 100, true, 0.25f);
	m_ProjectiveMotionRL.ProjectiveMotionRLDeblurTVReg(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 100, true, 0.125f);
	m_ProjectiveMotionRL.ProjectiveMotionRLDeblur(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 100, true);
	RMSError =	(m_ProjectiveMotionRL.ComputeRMSError(fImg[0], deblurImg[0], width, height) +
				m_ProjectiveMotionRL.ComputeRMSError(fImg[1], deblurImg[1], width, height) +
				m_ProjectiveMotionRL.ComputeRMSError(fImg[2], deblurImg[2], width, height))/3.0f;
	sprintf(fname, "%s_deblurTVReg_%f.bmp", prefix, RMSError*255.0f);
	printf("Done, RMS Error: %f\n", RMSError*255.0f);
	writeBMP(fname, width, height, deblurImg[0], deblurImg[1], deblurImg[2]);
	
	printf("Laplacian Regularization Algorithm:\n");
	memcpy(deblurImg[0], intermediatedeblurImg[0], width*height*sizeof(float));
	memcpy(deblurImg[1], intermediatedeblurImg[1], width*height*sizeof(float));
	memcpy(deblurImg[2], intermediatedeblurImg[2], width*height*sizeof(float));
	
//	m_ProjectiveMotionRL.ProjectiveMotionRLDeblurSpsReg(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 500, true, 0.5f);
	m_ProjectiveMotionRL.ProjectiveMotionRLDeblurSpsReg(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 100, true, 1.0f);
	m_ProjectiveMotionRL.ProjectiveMotionRLDeblurSpsReg(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 100, true, 0.5f);
	m_ProjectiveMotionRL.ProjectiveMotionRLDeblurSpsReg(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 100, true, 0.25f);
	m_ProjectiveMotionRL.ProjectiveMotionRLDeblurSpsReg(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 100, true, 0.125f);
	m_ProjectiveMotionRL.ProjectiveMotionRLDeblur(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 100, true);
	RMSError =	(m_ProjectiveMotionRL.ComputeRMSError(fImg[0], deblurImg[0], width, height) +
				m_ProjectiveMotionRL.ComputeRMSError(fImg[1], deblurImg[1], width, height) +
				m_ProjectiveMotionRL.ComputeRMSError(fImg[2], deblurImg[2], width, height))/3.0f;
	sprintf(fname, "%s_deblurSpsReg_%f.bmp", prefix, RMSError*255.0f);
	printf("Done, RMS Error: %f\n", RMSError*255.0f);
	writeBMP(fname, width, height, deblurImg[0], deblurImg[1], deblurImg[2]);
	
	printf("Bilateral Regularization Algorithm:\n");
	memcpy(deblurImg[0], intermediatedeblurImg[0], width*height*sizeof(float));
	memcpy(deblurImg[1], intermediatedeblurImg[1], width*height*sizeof(float));
	memcpy(deblurImg[2], intermediatedeblurImg[2], width*height*sizeof(float));
	
//	m_ProjectiveMotionRL.ProjectiveMotionRLDeblurBilateralReg(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 500, true, 0.5f);
	m_ProjectiveMotionRL.ProjectiveMotionRLDeblurBilateralReg(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 100, true, 1.0f);
	m_ProjectiveMotionRL.ProjectiveMotionRLDeblurBilateralReg(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 100, true, 0.5f);
	m_ProjectiveMotionRL.ProjectiveMotionRLDeblurBilateralReg(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 100, true, 0.25f);
	m_ProjectiveMotionRL.ProjectiveMotionRLDeblurBilateralReg(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 100, true, 0.125f);
	m_ProjectiveMotionRL.ProjectiveMotionRLDeblur(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 100, true);
	RMSError =	(m_ProjectiveMotionRL.ComputeRMSError(fImg[0], deblurImg[0], width, height) +
				m_ProjectiveMotionRL.ComputeRMSError(fImg[1], deblurImg[1], width, height) +
				m_ProjectiveMotionRL.ComputeRMSError(fImg[2], deblurImg[2], width, height))/3.0f;
	sprintf(fname, "%s_deblurBilateralReg_%f.bmp", prefix, RMSError*255.0f);
	printf("Done, RMS Error: %f\n", RMSError*255.0f);
	writeBMP(fname, width, height, deblurImg[0], deblurImg[1], deblurImg[2]);

	printf("Bilateral Laplacian Regularization Algorithm:\n");
	memcpy(deblurImg[0], intermediatedeblurImg[0], width*height*sizeof(float));
	memcpy(deblurImg[1], intermediatedeblurImg[1], width*height*sizeof(float));
	memcpy(deblurImg[2], intermediatedeblurImg[2], width*height*sizeof(float));
	
//	m_ProjectiveMotionRL.ProjectiveMotionRLDeblurBilateralLapReg(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 500, true, 0.5f);
	m_ProjectiveMotionRL.ProjectiveMotionRLDeblurBilateralLapReg(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 100, true, 1.0f);
	m_ProjectiveMotionRL.ProjectiveMotionRLDeblurBilateralLapReg(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 100, true, 0.5f);
	m_ProjectiveMotionRL.ProjectiveMotionRLDeblurBilateralLapReg(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 100, true, 0.25f);
	m_ProjectiveMotionRL.ProjectiveMotionRLDeblurBilateralLapReg(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 100, true, 0.125f);
	m_ProjectiveMotionRL.ProjectiveMotionRLDeblur(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 100, true);
	RMSError =	(m_ProjectiveMotionRL.ComputeRMSError(fImg[0], deblurImg[0], width, height) +
				m_ProjectiveMotionRL.ComputeRMSError(fImg[1], deblurImg[1], width, height) +
				m_ProjectiveMotionRL.ComputeRMSError(fImg[2], deblurImg[2], width, height))/3.0f;
	sprintf(fname, "%s_deblurBilateralLapReg_%f.bmp", prefix, RMSError*255.0f);
	printf("Done, RMS Error: %f\n", RMSError*255.0f);
	writeBMP(fname, width, height, deblurImg[0], deblurImg[1], deblurImg[2]);

/*	printf("Testing for Convergence\n");
	memcpy(deblurImg[0], intermediatedeblurImg[0], width*height*sizeof(float));
	memcpy(deblurImg[1], intermediatedeblurImg[1], width*height*sizeof(float));
	memcpy(deblurImg[2], intermediatedeblurImg[2], width*height*sizeof(float));
	sprintf(fname, "ConvergencePoisson%s.txt", prefix);
	FILE* fp = fopen(fname, "w");
	for(int iteration = 0; iteration < 5000; iteration++){
		m_ProjectiveMotionRL.ProjectiveMotionRLDeblur(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 1, true);
		RMSError =	(m_ProjectiveMotionRL.ComputeRMSError(fImg[0], deblurImg[0], width, height) +
					m_ProjectiveMotionRL.ComputeRMSError(fImg[1], deblurImg[1], width, height) +
					m_ProjectiveMotionRL.ComputeRMSError(fImg[2], deblurImg[2], width, height))/3.0f;
		fprintf(fp, "%.12f\n", RMSError * 255.0f);
		if(iteration%100 == 0 || iteration == 20 || iteration == 50){
			sprintf(fname, "%s_deblurBasic_pitr%d_%f.bmp", prefix, iteration, RMSError*255.0f);
			writeBMP(fname, width, height, deblurImg[0], deblurImg[1], deblurImg[2]);
		}
	}
	fclose(fp);

	memcpy(deblurImg[0], intermediatedeblurImg[0], width*height*sizeof(float));
	memcpy(deblurImg[1], intermediatedeblurImg[1], width*height*sizeof(float));
	memcpy(deblurImg[2], intermediatedeblurImg[2], width*height*sizeof(float));
	sprintf(fname, "ConvergenceGaussian%s.txt", prefix);
	fp = fopen(fname, "w");
	for(int iteration = 0; iteration < 5000; iteration++){
		m_ProjectiveMotionRL.ProjectiveMotionRLDeblur(bImg[0], bImg[1], bImg[2], blurwidth, blurheight, deblurImg[0], deblurImg[1], deblurImg[2], width, height, 1, false);
		RMSError =	(m_ProjectiveMotionRL.ComputeRMSError(fImg[0], deblurImg[0], width, height) +
					m_ProjectiveMotionRL.ComputeRMSError(fImg[1], deblurImg[1], width, height) +
					m_ProjectiveMotionRL.ComputeRMSError(fImg[2], deblurImg[2], width, height))/3.0f;
		fprintf(fp, "%.12f\n", RMSError * 255.0f);
		if(iteration%100 == 0 || iteration == 20 || iteration == 50){
			sprintf(fname, "%s_deblurBasic_gitr%d_%f.bmp", prefix, iteration, RMSError*255.0f);
			writeBMP(fname, width, height, deblurImg[0], deblurImg[1], deblurImg[2]);
		}
	}
	fclose(fp);
*/
	delete [] fImg[0];
	delete [] fImg[1];
	delete [] fImg[2];
	delete [] bImg[0];
	delete [] bImg[1];
	delete [] bImg[2];
	//delete [] inputWeight;
	delete [] outputWeight;
	delete [] deblurImg[0];
	delete [] deblurImg[1];
	delete [] deblurImg[2];
	delete [] intermediatedeblurImg[0];
	delete [] intermediatedeblurImg[1];
	delete [] intermediatedeblurImg[2];
}
