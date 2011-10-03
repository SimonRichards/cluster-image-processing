#include "ProjectiveMotionRL.h"
#include "BicubicInterpolation.h"
#include "bitmap.h"
#include "ImResize.h"
#include <memory>

void ProjectiveMotionRL::WarpImage(float* InputImg, float* inputWeight, int iwidth, int iheight, float* OutputImg, float* outputWeight, int width, int height, int i){
	int x,y,index;
	float fx, fy;
	if(i >= 0 && i < NumSamples){
		float woffset = width*0.5f, hoffset = height*0.5f;
		float iwoffset = iwidth*0.5f, ihoffset = iheight*0.5f;

		for(y = 0, index = 0; y < height; y++){
			for(x = 0; x < width; x++, index++){
				fx = x - woffset;
				fy = y - hoffset;
				IHmatrix[i].Transform(fx, fy); //Inverse mapping, use inverse instead
				fx += iwoffset;
				fy += ihoffset;

				if( fx >= 0 && fx < iwidth-1 && fy >= 0 && fy < iheight-1){
					if(inputWeight){
						outputWeight[index] = 0.01f + ReturnInterpolatedValueFast(fx, fy, inputWeight, iwidth, iheight);
					}else{
						outputWeight[index] = 1.01f;
					}
				}else{
					outputWeight[index] = 0.01f;
				}

				if( fx < 0 ) fx = 0;
				if( fy < 0 ) fy = 0;
				if( fx >= iwidth - 1.001f) fx = iwidth - 1.001f;
				if( fy >= iheight - 1.001f) fy = iheight - 1.001f;

				OutputImg[index] = ReturnInterpolatedValueFast(fx, fy, InputImg, iwidth, iheight);
			}
		}
	}else if(i < 0 && i > -NumSamples){
		float woffset = width*0.5f, hoffset = height*0.5f;
		float iwoffset = iwidth*0.5f, ihoffset = iheight*0.5f;

		for(y = 0, index = 0; y < height; y++){
			for(x = 0; x < width; x++, index++){
				fx = x - woffset;
				fy = y - hoffset;
				Hmatrix[-i].Transform(fx, fy); //Inverse mapping, use inverse instead
				fx += iwoffset;
				fy += ihoffset;

				if( fx >= 0 && fx < iwidth-1 && fy >= 0 && fy < iheight-1){
					if(inputWeight){
						outputWeight[index] = 0.01f + ReturnInterpolatedValueFast(fx, fy, inputWeight, iwidth, iheight);
					}else{
						outputWeight[index] = 1.01f;
					}
				}else{
					outputWeight[index] = 0.01f;
				}

				if( fx < 0 ) fx = 0;
				if( fy < 0 ) fy = 0;
				if( fx >= iwidth - 1.001f) fx = iwidth - 1.001f;
				if( fy >= iheight - 1.001f) fy = iheight - 1.001f;

				OutputImg[index] = ReturnInterpolatedValueFast(fx, fy, InputImg, iwidth, iheight);
			}
		}
	}
}

void ProjectiveMotionRL::WarpImage(float* InputImgR, float* InputImgG, float* InputImgB, float* inputWeight, int iwidth, int iheight, float* OutputImgR, float* OutputImgG, float* OutputImgB, float* outputWeight, int width, int height, int i){
	int x,y,index;
	float fx, fy;
	if(i >= 0 && i < NumSamples){
		float woffset = width*0.5f, hoffset = height*0.5f;
		float iwoffset = iwidth*0.5f, ihoffset = iheight*0.5f;

		for(y = 0, index = 0; y < height; y++){
			for(x = 0; x < width; x++, index++){
				fx = x - woffset;
				fy = y - hoffset;
				IHmatrix[i].Transform(fx, fy); //Inverse mapping, use inverse instead
				fx += iwoffset;
				fy += ihoffset;

				if( fx >= 0 && fx < iwidth-1 && fy >= 0 && fy < iheight-1){
					if(inputWeight){
						outputWeight[index] = 0.01f + ReturnInterpolatedValueFast(fx, fy, inputWeight, iwidth, iheight);
					}else{
						outputWeight[index] = 1.01f;
					}
				}else{
					outputWeight[index] = 0.01f;
				}

				if( fx < 0 ) fx = 0;
				if( fy < 0 ) fy = 0;
				if( fx >= iwidth - 1.001f) fx = iwidth - 1.001f;
				if( fy >= iheight - 1.001f) fy = iheight - 1.001f;

				ReturnInterpolatedValueFast(fx, fy, InputImgR, InputImgG, InputImgB, iwidth, iheight, OutputImgR[index], OutputImgG[index], OutputImgB[index]);
			}
		}
	}else if(i < 0 && i > -NumSamples){
		float woffset = width*0.5f, hoffset = height*0.5f;
		float iwoffset = iwidth*0.5f, ihoffset = iheight*0.5f;

		for(y = 0, index = 0; y < height; y++){
			for(x = 0; x < width; x++, index++){
				fx = x - woffset;
				fy = y - hoffset;
				Hmatrix[-i].Transform(fx, fy); //Inverse mapping, use inverse instead
				fx += iwoffset;
				fy += ihoffset;

				if( fx >= 0 && fx < iwidth-1 && fy >= 0 && fy < iheight-1){
					if(inputWeight){
						outputWeight[index] = 0.01f + ReturnInterpolatedValueFast(fx, fy, inputWeight, iwidth, iheight);
					}else{
						outputWeight[index] = 1.01f;
					}
				}else{
					outputWeight[index] = 0.01f;
				}

				if( fx < 0 ) fx = 0;
				if( fy < 0 ) fy = 0;
				if( fx >= iwidth - 1.001f) fx = iwidth - 1.001f;
				if( fy >= iheight - 1.001f) fy = iheight - 1.001f;

				ReturnInterpolatedValueFast(fx, fy, InputImgR, InputImgG, InputImgB, iwidth, iheight, OutputImgR[index], OutputImgG[index], OutputImgB[index]);
			}
		}
	}
}

void ProjectiveMotionRL::GenerateMotionBlurImg(float* InputImg, float* inputWeight, int iwidth, int iheight, float* BlurImg, float* outputWeight, int width, int height, bool bforward){
	int i,index, totalpixel = width*height;
	
	if(!WarpImgBuffer){
		SetBuffer(width, height);
	}
	
	memset(BlurImg, 0, totalpixel*sizeof(float));
	memset(outputWeight, 0, totalpixel*sizeof(float));
	for(i = 0; i < NumSamples; i++){
		if( bforward ){
			WarpImage(InputImg, inputWeight, iwidth, iheight, WarpImgBuffer, WarpWeightBuffer, width, height, i);
		}else{
			WarpImage(InputImg, inputWeight, iwidth, iheight, WarpImgBuffer, WarpWeightBuffer, width, height, -i);
		}
		for(index = 0; index < totalpixel; index++){
			BlurImg[index] += WarpImgBuffer[index] * WarpWeightBuffer[index];
			outputWeight[index] += WarpWeightBuffer[index];
		}
	}

	for(index = 0; index < totalpixel; index++){
		BlurImg[index] /= outputWeight[index];
	}
}

void ProjectiveMotionRL::GenerateMotionBlurImg(float* InputImgR, float* InputImgG, float* InputImgB, float* inputWeight, int iwidth, int iheight, float* BlurImgR, float* BlurImgG, float* BlurImgB, float* outputWeight, int width, int height, bool bforward){
	int i,index, totalpixel = width*height;
	
	if(!WarpImgBuffer){
		SetBuffer(width, height);
	}
	
	memset(BlurImgR, 0, totalpixel*sizeof(float));
	memset(BlurImgG, 0, totalpixel*sizeof(float));
	memset(BlurImgB, 0, totalpixel*sizeof(float));
	memset(outputWeight, 0, totalpixel*sizeof(float));
	for(i = 0; i < NumSamples; i++){
		if( bforward ){
			WarpImage(InputImgR, InputImgG, InputImgB, inputWeight, iwidth, iheight, WarpImgBufferR, WarpImgBufferG, WarpImgBufferB, WarpWeightBuffer, width, height, i);
		}else{
			WarpImage(InputImgR, InputImgG, InputImgB, inputWeight, iwidth, iheight, WarpImgBufferR, WarpImgBufferG, WarpImgBufferB, WarpWeightBuffer, width, height, -i);
		}
		for(index = 0; index < totalpixel; index++){
			BlurImgR[index] += WarpImgBufferR[index] * WarpWeightBuffer[index];
			BlurImgG[index] += WarpImgBufferG[index] * WarpWeightBuffer[index];
			BlurImgB[index] += WarpImgBufferB[index] * WarpWeightBuffer[index];
			outputWeight[index] += WarpWeightBuffer[index];
		}
	}

	for(index = 0; index < totalpixel; index++){
		BlurImgR[index] /= outputWeight[index];
		BlurImgG[index] /= outputWeight[index];
		BlurImgB[index] /= outputWeight[index];
	}
}

void ProjectiveMotionRL::ProjectiveMotionRLDeblur(float* BlurImg, int iwidth, int iheight, float* DeblurImg, int width, int height, int Niter, bool bPoisson){
	int x,y,index, itr;
	float* InputWeight = NULL;

	float* DeltaImg = new float[iwidth*iheight];

	ClearBuffer();
	if(width*height >= iwidth*iheight )
		SetBuffer(width, height);
	else
		SetBuffer(iwidth, iheight);	
	
	for(itr = 0; itr < Niter; itr++){
		GenerateMotionBlurImg(DeblurImg, InputWeight, width, height, BlurImgBuffer, BlurWeightBuffer, iwidth, iheight, true);
		for(y = 0, index = 0; y < iheight; y++){
			for(x = 0; x < iwidth; x++, index++){
				if(bPoisson){
					if( BlurImgBuffer[index] > 0.001f){
						DeltaImg[index] = BlurImg[index] / BlurImgBuffer[index];
					}else{
						DeltaImg[index] = BlurImg[index] / 0.001f;
					}
				}else{
					DeltaImg[index] = BlurImg[index] - BlurImgBuffer[index];
				}
			}
		}
		GenerateMotionBlurImg(DeltaImg, BlurWeightBuffer, iwidth, iheight, ErrorImgBuffer, ErrorWeightBuffer, width, height, false);
		for(y = 0, index = 0; y < height; y++){
			for(x = 0; x < width; x++, index++){
				if(bPoisson){
					DeblurImg[index] *= ErrorImgBuffer[index];
				}else{
					DeblurImg[index] += ErrorImgBuffer[index];
				}
				if(DeblurImg[index] < 0) DeblurImg[index] = 0;
				if(DeblurImg[index] > 1) DeblurImg[index] = 1;
			}
		}

#ifdef __SHOWERROR__
		printf("RMS Error: %f\n", ComputeRMSError(GroundTruthImg, DeblurImg, width, height));
#else
		printf(".");
#endif
	}

	delete [] DeltaImg;
}

void ProjectiveMotionRL::ProjectiveMotionRLDeblur(float* BlurImgR, float* BlurImgG, float* BlurImgB, int iwidth, int iheight, float* DeblurImgR, float* DeblurImgG, float* DeblurImgB, int width, int height, int Niter, bool bPoisson){
	int x,y,index, itr;
	float* InputWeight = NULL;

	float* DeltaImgR = new float[iwidth*iheight];
	float* DeltaImgG = new float[iwidth*iheight];
	float* DeltaImgB = new float[iwidth*iheight];

	ClearBuffer();
	if(width*height >= iwidth*iheight )
		SetBuffer(width, height);
	else
		SetBuffer(iwidth, iheight);
	
	
	for(itr = 0; itr < Niter; itr++){
		GenerateMotionBlurImg(DeblurImgR, DeblurImgG, DeblurImgB, InputWeight, width, height, BlurImgBufferR, BlurImgBufferG, BlurImgBufferB, BlurWeightBuffer, iwidth, iheight, true);
		for(y = 0, index = 0; y < iheight; y++){
			for(x = 0; x < iwidth; x++, index++){
				if(bPoisson){
					if( BlurImgBufferR[index] > 0.001f){
						DeltaImgR[index] = BlurImgR[index] / BlurImgBufferR[index];
					}else{
						DeltaImgR[index] = BlurImgR[index] / 0.001f;
					}
					if( BlurImgBufferG[index] > 0.001f){
						DeltaImgG[index] = BlurImgG[index] / BlurImgBufferG[index];
					}else{
						DeltaImgG[index] = BlurImgG[index] / 0.001f;
					}
					if( BlurImgBufferB[index] > 0.001f){
						DeltaImgB[index] = BlurImgB[index] / BlurImgBufferB[index];
					}else{
						DeltaImgB[index] = BlurImgB[index] / 0.001f;
					}
				}else{
					DeltaImgR[index] = BlurImgR[index] - BlurImgBufferR[index];
					DeltaImgG[index] = BlurImgG[index] - BlurImgBufferG[index];
					DeltaImgB[index] = BlurImgB[index] - BlurImgBufferB[index];
				}
			}
		}
		GenerateMotionBlurImg(DeltaImgR, DeltaImgG, DeltaImgB, BlurWeightBuffer, iwidth, iheight, ErrorImgBufferR, ErrorImgBufferG, ErrorImgBufferB, ErrorWeightBuffer, width, height, false);
		for(y = 0, index = 0; y < height; y++){
			for(x = 0; x < width; x++, index++){
				if(bPoisson){
					DeblurImgR[index] *= ErrorImgBufferR[index];
					DeblurImgG[index] *= ErrorImgBufferG[index];
					DeblurImgB[index] *= ErrorImgBufferB[index];
				}else{
					DeblurImgR[index] += ErrorImgBufferR[index];
					DeblurImgG[index] += ErrorImgBufferG[index];
					DeblurImgB[index] += ErrorImgBufferB[index];
				}
				if(DeblurImgR[index] < 0) DeblurImgR[index] = 0;
				if(DeblurImgR[index] > 1) DeblurImgR[index] = 1;
				if(DeblurImgG[index] < 0) DeblurImgG[index] = 0;
				if(DeblurImgG[index] > 1) DeblurImgG[index] = 1;
				if(DeblurImgB[index] < 0) DeblurImgB[index] = 0;
				if(DeblurImgB[index] > 1) DeblurImgB[index] = 1;
			}
		}
#ifdef __SHOWERROR__
		printf("RMS Error R: %f\n", ComputeRMSError(GroundTruthImgR, DeblurImgR, width, height));
		printf("RMS Error G: %f\n", ComputeRMSError(GroundTruthImgG, DeblurImgG, width, height));
		printf("RMS Error B: %f\n", ComputeRMSError(GroundTruthImgB, DeblurImgB, width, height));
#else
		printf(".");
#endif
	}

	delete [] DeltaImgR;
	delete [] DeltaImgG;
	delete [] DeltaImgB;
}

void ProjectiveMotionRL::ProjectiveMotionRLDeblurTVReg(float* BlurImg, int iwidth, int iheight, float* DeblurImg, int width, int height, int Niter, bool bPoisson, float lambda){
	int x,y,index, itr;
	float* InputWeight = NULL;
	
	float* DeltaImg = new float[iwidth*iheight];
	float* DxImg = new float[width*height];
	float* DyImg = new float[width*height];
	float* DxxImg = new float[width*height];
	float* DyyImg = new float[width*height];

	ClearBuffer();
	if(width*height >= iwidth*iheight )
		SetBuffer(width, height);
	else
		SetBuffer(iwidth, iheight);
	
	for(itr = 0; itr < Niter; itr++){
		GenerateMotionBlurImg(DeblurImg, InputWeight, width, height, BlurImgBuffer, BlurWeightBuffer, iwidth, iheight, true);
		for(y = 0, index = 0; y < iheight; y++){
			for(x = 0; x < iwidth; x++, index++){
				if(bPoisson){
					if( BlurImgBuffer[index] > 0.001f){
						DeltaImg[index] = BlurImg[index] / BlurImgBuffer[index];
					}else{
						DeltaImg[index] = BlurImg[index] / 0.001f;
					}
				}else{
					DeltaImg[index] = BlurImg[index] - BlurImgBuffer[index];
				}
			}
		}
		GenerateMotionBlurImg(DeltaImg, BlurWeightBuffer, iwidth, iheight, ErrorImgBuffer, ErrorWeightBuffer, width, height, false);
		
		ComputeGradientImage(DeblurImg, width, height, DxImg, DyImg, true);
		for(y = 0, index = 0; y < height; y++){ //Normalize the gradient
			for(x = 0; x < width; x++, index++){
				if(DxImg[index] > 0) DxImg[index] = 1.0f / 255.0f;
				if(DxImg[index] < 0) DxImg[index] = -1.0f / 255.0f;
				if(DyImg[index] > 0) DyImg[index] = 1.0f / 255.0f;
				if(DyImg[index] < 0) DyImg[index] = -1.0f / 255.0f;
			}
		}
		ComputeGradientXImage(DxImg, width, height, DxxImg, false);
		ComputeGradientYImage(DyImg, width, height, DyyImg, false);
		
		for(y = 0, index = 0; y < height; y++){
			for(x = 0; x < width; x++, index++){
				if(bPoisson){
					DeblurImg[index] *= ErrorImgBuffer[index] /(1 + lambda * (DxxImg[index] + DyyImg[index]) );
				}else{
					DeblurImg[index] += ErrorImgBuffer[index] - lambda * (DxxImg[index] + DyyImg[index]);
				}
				if(DeblurImg[index] < 0) DeblurImg[index] = 0;
				if(DeblurImg[index] > 1) DeblurImg[index] = 1;
			}
		}
		
#ifdef __SHOWERROR__
		printf("RMS Error: %f\n", ComputeRMSError(GroundTruthImg, DeblurImg, width, height));
#else
		printf(".");
#endif
	}

	delete [] DeltaImg;

	delete [] DxImg;
	delete [] DyImg;
	delete [] DxxImg;
	delete [] DyyImg;
}

void ProjectiveMotionRL::ProjectiveMotionRLDeblurTVReg(float* BlurImgR, float* BlurImgG, float* BlurImgB, int iwidth, int iheight, float* DeblurImgR, float* DeblurImgG, float* DeblurImgB, int width, int height, int Niter, bool bPoisson, float lambda){
	int x,y,index, itr;
	float* InputWeight = NULL;

	float* DeltaImgR = new float[iwidth*iheight];
	float* DeltaImgG = new float[iwidth*iheight];
	float* DeltaImgB = new float[iwidth*iheight];

	float* DxImgR = new float[width*height];
	float* DyImgR = new float[width*height];
	float* DxxImgR = new float[width*height];
	float* DyyImgR = new float[width*height];
	float* DxImgG = new float[width*height];
	float* DyImgG = new float[width*height];
	float* DxxImgG = new float[width*height];
	float* DyyImgG = new float[width*height];
	float* DxImgB = new float[width*height];
	float* DyImgB = new float[width*height];
	float* DxxImgB = new float[width*height];
	float* DyyImgB = new float[width*height];

	ClearBuffer();
	if(width*height >= iwidth*iheight )
		SetBuffer(width, height);
	else
		SetBuffer(iwidth, iheight);
	
	for(itr = 0; itr < Niter; itr++){
		GenerateMotionBlurImg(DeblurImgR, DeblurImgG, DeblurImgB, InputWeight, width, height, BlurImgBufferR, BlurImgBufferG, BlurImgBufferB, BlurWeightBuffer, iwidth, iheight, true);
		for(y = 0, index = 0; y < iheight; y++){
			for(x = 0; x < iwidth; x++, index++){
				if(bPoisson){
					if( BlurImgBufferR[index] > 0.001f){
						DeltaImgR[index] = BlurImgR[index] / BlurImgBufferR[index];
					}else{
						DeltaImgR[index] = BlurImgR[index] / 0.001f;
					}
					if( BlurImgBufferG[index] > 0.001f){
						DeltaImgG[index] = BlurImgG[index] / BlurImgBufferG[index];
					}else{
						DeltaImgG[index] = BlurImgG[index] / 0.001f;
					}
					if( BlurImgBufferB[index] > 0.001f){
						DeltaImgB[index] = BlurImgB[index] / BlurImgBufferB[index];
					}else{
						DeltaImgB[index] = BlurImgB[index] / 0.001f;
					}
				}else{
					DeltaImgR[index] = BlurImgR[index] - BlurImgBufferR[index];
					DeltaImgG[index] = BlurImgG[index] - BlurImgBufferG[index];
					DeltaImgB[index] = BlurImgB[index] - BlurImgBufferB[index];
				}
			}
		}
		GenerateMotionBlurImg(DeltaImgR, DeltaImgG, DeltaImgB, BlurWeightBuffer, iwidth, iheight, ErrorImgBufferR, ErrorImgBufferG, ErrorImgBufferB, ErrorWeightBuffer, width, height, false);
		
		ComputeGradientImage(DeblurImgR, width, height, DxImgR, DyImgR, true);
		ComputeGradientImage(DeblurImgG, width, height, DxImgG, DyImgG, true);
		ComputeGradientImage(DeblurImgB, width, height, DxImgB, DyImgB, true);
		for(y = 0, index = 0; y < height; y++){
			for(x = 0; x < width; x++, index++){
				if(DxImgR[index] > 0) DxImgR[index] = 1.0f / 255.0f;
				if(DxImgR[index] < 0) DxImgR[index] = -1.0f / 255.0f;
				if(DyImgR[index] > 0) DyImgR[index] = 1.0f / 255.0f;
				if(DyImgR[index] < 0) DyImgR[index] = -1.0f / 255.0f;
				if(DxImgG[index] > 0) DxImgG[index] = 1.0f / 255.0f;
				if(DxImgG[index] < 0) DxImgG[index] = -1.0f / 255.0f;
				if(DyImgG[index] > 0) DyImgG[index] = 1.0f / 255.0f;
				if(DyImgG[index] < 0) DyImgG[index] = -1.0f / 255.0f;
				if(DxImgB[index] > 0) DxImgB[index] = 1.0f / 255.0f;
				if(DxImgB[index] < 0) DxImgB[index] = -1.0f / 255.0f;
				if(DyImgB[index] > 0) DyImgB[index] = 1.0f / 255.0f;
				if(DyImgB[index] < 0) DyImgB[index] = -1.0f / 255.0f;
			}
		}
		ComputeGradientXImage(DxImgR, width, height, DxxImgR, false);
		ComputeGradientYImage(DyImgR, width, height, DyyImgR, false);
		ComputeGradientXImage(DxImgG, width, height, DxxImgG, false);
		ComputeGradientYImage(DyImgG, width, height, DyyImgG, false);
		ComputeGradientXImage(DxImgB, width, height, DxxImgB, false);
		ComputeGradientYImage(DyImgB, width, height, DyyImgB, false);
		
		for(y = 0, index = 0; y < height; y++){
			for(x = 0; x < width; x++, index++){
				if(bPoisson){
					DeblurImgR[index] *= ErrorImgBufferR[index] /(1 + lambda * (DxxImgR[index] + DyyImgR[index]) );
					DeblurImgG[index] *= ErrorImgBufferG[index] /(1 + lambda * (DxxImgG[index] + DyyImgG[index]) );
					DeblurImgB[index] *= ErrorImgBufferB[index] /(1 + lambda * (DxxImgB[index] + DyyImgB[index]) );
				}else{
					DeblurImgR[index] += ErrorImgBufferR[index] - lambda * (DxxImgR[index] + DyyImgR[index]);
					DeblurImgG[index] += ErrorImgBufferG[index] - lambda * (DxxImgG[index] + DyyImgG[index]);
					DeblurImgB[index] += ErrorImgBufferB[index] - lambda * (DxxImgB[index] + DyyImgB[index]);
				}
				if(DeblurImgR[index] < 0) DeblurImgR[index] = 0;
				if(DeblurImgR[index] > 1) DeblurImgR[index] = 1;
				if(DeblurImgG[index] < 0) DeblurImgG[index] = 0;
				if(DeblurImgG[index] > 1) DeblurImgG[index] = 1;
				if(DeblurImgB[index] < 0) DeblurImgB[index] = 0;
				if(DeblurImgB[index] > 1) DeblurImgB[index] = 1;
			}
		}
		
#ifdef __SHOWERROR__
		printf("RMS Error R: %f\n", ComputeRMSError(GroundTruthImgR, DeblurImgR, width, height));
		printf("RMS Error G: %f\n", ComputeRMSError(GroundTruthImgG, DeblurImgG, width, height));
		printf("RMS Error B: %f\n", ComputeRMSError(GroundTruthImgB, DeblurImgB, width, height));
#else
		printf(".");
#endif
	}

	delete [] DeltaImgR;
	delete [] DeltaImgG;
	delete [] DeltaImgB;

	delete [] DxImgR;
	delete [] DyImgR;
	delete [] DxxImgR;
	delete [] DyyImgR;
	delete [] DxImgG;
	delete [] DyImgG;
	delete [] DxxImgG;
	delete [] DyyImgG;
	delete [] DxImgB;
	delete [] DyImgB;
	delete [] DxxImgB;
	delete [] DyyImgB;
}

void ProjectiveMotionRL::ProjectiveMotionRLDeblurSpsReg(float* BlurImg, int iwidth, int iheight, float* DeblurImg, int width, int height, int Niter, bool bPoisson, float lambda){
	int x,y,index, itr;
	float* InputWeight = NULL;
	float Wx, Wy;

	float* DeltaImg = new float[iwidth*iheight];

	float* DxImg = new float[width*height];
	float* DyImg = new float[width*height];
	float* DxxImg = new float[width*height];
	float* DyyImg = new float[width*height];

	ClearBuffer();
	if(width*height >= iwidth*iheight )
		SetBuffer(width, height);
	else
		SetBuffer(iwidth, iheight);
	
	for(itr = 0; itr < Niter; itr++){
		GenerateMotionBlurImg(DeblurImg, InputWeight, width, height, BlurImgBuffer, BlurWeightBuffer, iwidth, iheight, true);
		for(y = 0, index = 0; y < iheight; y++){
			for(x = 0; x < iwidth; x++, index++){
				if(bPoisson){
					if( BlurImgBuffer[index] > 0.001f){
						DeltaImg[index] = BlurImg[index] / BlurImgBuffer[index];
					}else{
						DeltaImg[index] = BlurImg[index] / 0.001f;
					}
				}else{
					DeltaImg[index] = BlurImg[index] - BlurImgBuffer[index];
				}
			}
		}
		GenerateMotionBlurImg(DeltaImg, BlurWeightBuffer, iwidth, iheight, ErrorImgBuffer, ErrorWeightBuffer, width, height, false);
		
		ComputeGradientImage(DeblurImg, width, height, DxImg, DyImg, true);
		
		ComputeGradientXImage(DxImg, width, height, DxxImg, false);
		ComputeGradientYImage(DyImg, width, height, DyyImg, false);
		
		for(y = 0, index = 0; y < height; y++){
			for(x = 0; x < width; x++, index++){
				Wx = SpsTable[(int)(fabs(DxImg[index])*255.0f)];
				Wy = SpsTable[(int)(fabs(DyImg[index])*255.0f)]; 
				
				if(bPoisson){
					DeblurImg[index] = DeblurImg[index] /(1 - lambda * (Wx * DxxImg[index] + Wy * DyyImg[index]) ) * ErrorImgBuffer[index];
				}else{
					DeblurImg[index] = DeblurImg[index] + ErrorImgBuffer[index] - lambda * (Wx * DxxImg[index] + Wy * DyyImg[index]);
				}
				if(DeblurImg[index] < 0) DeblurImg[index] = 0;
				if(DeblurImg[index] > 1) DeblurImg[index] = 1;
			}
		}
		
#ifdef __SHOWERROR__
		printf("RMS Error: %f\n", ComputeRMSError(GroundTruthImg, DeblurImg, width, height));
#else
		printf(".");
#endif
	}

	delete [] DeltaImg;
	delete [] DxImg;
	delete [] DyImg;
	delete [] DxxImg;
	delete [] DyyImg;
}

void ProjectiveMotionRL::ProjectiveMotionRLDeblurSpsReg(float* BlurImgR, float* BlurImgG, float* BlurImgB, int iwidth, int iheight, float* DeblurImgR, float* DeblurImgG, float* DeblurImgB, int width, int height, int Niter, bool bPoisson, float lambda){
	int x,y,index, itr;
	float* InputWeight = NULL;
	float WxR, WyR, WxG, WyG, WxB, WyB;

	float* DeltaImgR = new float[iwidth*iheight];
	float* DeltaImgG = new float[iwidth*iheight];
	float* DeltaImgB = new float[iwidth*iheight];

	float* DxImgR = new float[width*height];
	float* DyImgR = new float[width*height];
	float* DxxImgR = new float[width*height];
	float* DyyImgR = new float[width*height];
	float* DxImgG = new float[width*height];
	float* DyImgG = new float[width*height];
	float* DxxImgG = new float[width*height];
	float* DyyImgG = new float[width*height];
	float* DxImgB = new float[width*height];
	float* DyImgB = new float[width*height];
	float* DxxImgB = new float[width*height];
	float* DyyImgB = new float[width*height];

	ClearBuffer();
	if(width*height >= iwidth*iheight )
		SetBuffer(width, height);
	else
		SetBuffer(iwidth, iheight);
	
	for(itr = 0; itr < Niter; itr++){
		GenerateMotionBlurImg(DeblurImgR, DeblurImgG, DeblurImgB, InputWeight, width, height, BlurImgBufferR, BlurImgBufferG, BlurImgBufferB, BlurWeightBuffer, iwidth, iheight, true);
		for(y = 0, index = 0; y < iheight; y++){
			for(x = 0; x < iwidth; x++, index++){
				if(bPoisson){
					if( BlurImgBufferR[index] > 0.001f){
						DeltaImgR[index] = BlurImgR[index] / BlurImgBufferR[index];
					}else{
						DeltaImgR[index] = BlurImgR[index] / 0.001f;
					}
					if( BlurImgBufferG[index] > 0.001f){
						DeltaImgG[index] = BlurImgG[index] / BlurImgBufferG[index];
					}else{
						DeltaImgG[index] = BlurImgG[index] / 0.001f;
					}
					if( BlurImgBufferB[index] > 0.001f){
						DeltaImgB[index] = BlurImgB[index] / BlurImgBufferB[index];
					}else{
						DeltaImgB[index] = BlurImgB[index] / 0.001f;
					}
				}else{
					DeltaImgR[index] = BlurImgR[index] - BlurImgBufferR[index];
					DeltaImgG[index] = BlurImgG[index] - BlurImgBufferG[index];
					DeltaImgB[index] = BlurImgB[index] - BlurImgBufferB[index];
				}
			}
		}
		GenerateMotionBlurImg(DeltaImgR, DeltaImgG, DeltaImgB, BlurWeightBuffer, iwidth, iheight, ErrorImgBufferR, ErrorImgBufferG, ErrorImgBufferB, ErrorWeightBuffer, width, height, false);
		
		ComputeGradientImage(DeblurImgR, width, height, DxImgR, DyImgR, true);
		ComputeGradientImage(DeblurImgG, width, height, DxImgG, DyImgG, true);
		ComputeGradientImage(DeblurImgB, width, height, DxImgB, DyImgB, true);
		
		ComputeGradientXImage(DxImgR, width, height, DxxImgR, false);
		ComputeGradientYImage(DyImgR, width, height, DyyImgR, false);
		ComputeGradientXImage(DxImgG, width, height, DxxImgG, false);
		ComputeGradientYImage(DyImgG, width, height, DyyImgG, false);
		ComputeGradientXImage(DxImgB, width, height, DxxImgB, false);
		ComputeGradientYImage(DyImgB, width, height, DyyImgB, false);
		
		for(y = 0, index = 0; y < height; y++){
			for(x = 0; x < width; x++, index++){
				WxR = SpsTable[(int)(fabs(DxImgR[index])*255.0f)];
				WyR = SpsTable[(int)(fabs(DyImgR[index])*255.0f)]; 
				WxG = SpsTable[(int)(fabs(DxImgG[index])*255.0f)];
				WyG = SpsTable[(int)(fabs(DyImgG[index])*255.0f)]; 
				WxB = SpsTable[(int)(fabs(DxImgB[index])*255.0f)];
				WyB = SpsTable[(int)(fabs(DyImgB[index])*255.0f)]; 			
				
				if(bPoisson){
					DeblurImgR[index] *= ErrorImgBufferR[index] /(1 + lambda * (WxR * DxxImgR[index] + WyR * DyyImgR[index]) );
					DeblurImgG[index] *= ErrorImgBufferG[index] /(1 + lambda * (WxG * DxxImgG[index] + WyG * DyyImgG[index]) );
					DeblurImgB[index] *= ErrorImgBufferB[index] /(1 + lambda * (WxB * DxxImgB[index] + WyB * DyyImgB[index]) );
				}else{
					DeblurImgR[index] += ErrorImgBufferR[index] - lambda * (WxR * DxxImgR[index] + WyR * DyyImgR[index]);
					DeblurImgG[index] += ErrorImgBufferG[index] - lambda * (WxG * DxxImgG[index] + WyG * DyyImgG[index]);
					DeblurImgB[index] += ErrorImgBufferB[index] - lambda * (WxB * DxxImgB[index] + WyB * DyyImgB[index]);
				}
				if(DeblurImgR[index] < 0) DeblurImgR[index] = 0;
				if(DeblurImgR[index] > 1) DeblurImgR[index] = 1;
				if(DeblurImgG[index] < 0) DeblurImgG[index] = 0;
				if(DeblurImgG[index] > 1) DeblurImgG[index] = 1;
				if(DeblurImgB[index] < 0) DeblurImgB[index] = 0;
				if(DeblurImgB[index] > 1) DeblurImgB[index] = 1;
			}
		}
		
#ifdef __SHOWERROR__
		printf("RMS Error R: %f\n", ComputeRMSError(GroundTruthImgR, DeblurImgR, width, height));
		printf("RMS Error G: %f\n", ComputeRMSError(GroundTruthImgG, DeblurImgG, width, height));
		printf("RMS Error B: %f\n", ComputeRMSError(GroundTruthImgB, DeblurImgB, width, height));
#else
		printf(".");
#endif
	}

	delete [] DeltaImgR;
	delete [] DxImgR;
	delete [] DyImgR;
	delete [] DxxImgR;
	delete [] DyyImgR;
	delete [] DeltaImgG;
	delete [] DxImgG;
	delete [] DyImgG;
	delete [] DxxImgG;
	delete [] DyyImgG;
	delete [] DeltaImgB;
	delete [] DxImgB;
	delete [] DyImgB;
	delete [] DxxImgB;
	delete [] DyyImgB;
}

void ProjectiveMotionRL::ProjectiveMotionRLDeblurBilateralReg(float* BlurImg, int iwidth, int iheight, float* DeblurImg, int width, int height, int Niter, bool bPoisson, float lambda){
	int x,y,index, itr;
	float* InputWeight = NULL;

	float* DeltaImg = new float[iwidth*iheight];
	float* BilateralRegImg = new float[width*height];

	ClearBuffer();
	if(width*height >= iwidth*iheight )
		SetBuffer(width, height);
	else
		SetBuffer(iwidth, iheight);
	
	for(itr = 0; itr < Niter; itr++){
		GenerateMotionBlurImg(DeblurImg, InputWeight, width, height, BlurImgBuffer, BlurWeightBuffer, iwidth, iheight, true);
		for(y = 0, index = 0; y < iheight; y++){
			for(x = 0; x < iwidth; x++, index++){
				if(bPoisson){
					if( BlurImgBuffer[index] > 0.001f){
						DeltaImg[index] = BlurImg[index] / BlurImgBuffer[index];
					}else{
						DeltaImg[index] = BlurImg[index] / 0.001f;
					}
				}else{
					DeltaImg[index] = BlurImg[index] - BlurImgBuffer[index];
				}
			}
		}
		GenerateMotionBlurImg(DeltaImg, BlurWeightBuffer, iwidth, iheight, ErrorImgBuffer, ErrorWeightBuffer, width, height, false);
		ComputeBilaterRegImage(DeblurImg, width, height, BilateralRegImg);

		for(y = 0, index = 0; y < height; y++){
			for(x = 0; x < width; x++, index++){
				if(bPoisson){
					DeblurImg[index] *= ErrorImgBuffer[index] /(1 + lambda * BilateralRegImg[index] );
				}else{
					DeblurImg[index] += ErrorImgBuffer[index] - lambda * BilateralRegImg[index];
				}
				if(DeblurImg[index] < 0) DeblurImg[index] = 0;
				if(DeblurImg[index] > 1) DeblurImg[index] = 1;
			}
		}
		
#ifdef __SHOWERROR__
		printf("RMS Error: %f\n", ComputeRMSError(GroundTruthImg, DeblurImg, width, height));
#else
		printf(".");
#endif
	}

	delete [] DeltaImg;
	delete [] BilateralRegImg;
}

void ProjectiveMotionRL::ProjectiveMotionRLDeblurBilateralReg(float* BlurImgR, float* BlurImgG, float* BlurImgB, int iwidth, int iheight, float* DeblurImgR, float* DeblurImgG, float* DeblurImgB, int width, int height, int Niter, bool bPoisson, float lambda){
	int x,y,index, itr;
	float* InputWeight = NULL;

	float* DeltaImgR = new float[iwidth*iheight];
	float* DeltaImgG = new float[iwidth*iheight];
	float* DeltaImgB = new float[iwidth*iheight];
	float* BilateralRegImgR = new float[width*height];
	float* BilateralRegImgG = new float[width*height];
	float* BilateralRegImgB = new float[width*height];

	ClearBuffer();
	if(width*height >= iwidth*iheight )
		SetBuffer(width, height);
	else
		SetBuffer(iwidth, iheight);
	
	for(itr = 0; itr < Niter; itr++){
		GenerateMotionBlurImg(DeblurImgR, DeblurImgG, DeblurImgB, InputWeight, width, height, BlurImgBufferR, BlurImgBufferG, BlurImgBufferB, BlurWeightBuffer, iwidth, iheight, true);
		for(y = 0, index = 0; y < iheight; y++){
			for(x = 0; x < iwidth; x++, index++){
				if(bPoisson){
					if( BlurImgBufferR[index] > 0.001f){
						DeltaImgR[index] = BlurImgR[index] / BlurImgBufferR[index];
					}else{
						DeltaImgR[index] = BlurImgR[index] / 0.001f;
					}
					if( BlurImgBufferG[index] > 0.001f){
						DeltaImgG[index] = BlurImgG[index] / BlurImgBufferG[index];
					}else{
						DeltaImgG[index] = BlurImgG[index] / 0.001f;
					}
					if( BlurImgBufferB[index] > 0.001f){
						DeltaImgB[index] = BlurImgB[index] / BlurImgBufferB[index];
					}else{
						DeltaImgB[index] = BlurImgB[index] / 0.001f;
					}
				}else{
					DeltaImgR[index] = BlurImgR[index] - BlurImgBufferR[index];
					DeltaImgG[index] = BlurImgG[index] - BlurImgBufferG[index];
					DeltaImgB[index] = BlurImgB[index] - BlurImgBufferB[index];
				}
			}
		}
		GenerateMotionBlurImg(DeltaImgR, DeltaImgG, DeltaImgB, BlurWeightBuffer, iwidth, iheight, ErrorImgBufferR, ErrorImgBufferG, ErrorImgBufferB, ErrorWeightBuffer, width, height, false);
		ComputeBilaterRegImage(DeblurImgR, width, height, BilateralRegImgR);
		ComputeBilaterRegImage(DeblurImgG, width, height, BilateralRegImgG);
		ComputeBilaterRegImage(DeblurImgB, width, height, BilateralRegImgB);

		for(y = 0, index = 0; y < height; y++){
			for(x = 0; x < width; x++, index++){
				if(bPoisson){
					DeblurImgR[index] *= ErrorImgBufferR[index] /(1 + lambda * BilateralRegImgR[index] );
					DeblurImgG[index] *= ErrorImgBufferG[index] /(1 + lambda * BilateralRegImgG[index] );
					DeblurImgB[index] *= ErrorImgBufferB[index] /(1 + lambda * BilateralRegImgB[index] );
				}else{
					DeblurImgR[index] += ErrorImgBufferR[index] - lambda * BilateralRegImgR[index];
					DeblurImgG[index] += ErrorImgBufferG[index] - lambda * BilateralRegImgG[index];
					DeblurImgB[index] += ErrorImgBufferB[index] - lambda * BilateralRegImgB[index];
				}
				if(DeblurImgR[index] < 0) DeblurImgR[index] = 0;
				if(DeblurImgR[index] > 1) DeblurImgR[index] = 1;
				if(DeblurImgG[index] < 0) DeblurImgG[index] = 0;
				if(DeblurImgG[index] > 1) DeblurImgG[index] = 1;
				if(DeblurImgB[index] < 0) DeblurImgB[index] = 0;
				if(DeblurImgB[index] > 1) DeblurImgB[index] = 1;
			}
		}
		
#ifdef __SHOWERROR__
		printf("RMS Error R: %f\n", ComputeRMSError(GroundTruthImgR, DeblurImgR, width, height));
		printf("RMS Error G: %f\n", ComputeRMSError(GroundTruthImgG, DeblurImgG, width, height));
		printf("RMS Error B: %f\n", ComputeRMSError(GroundTruthImgB, DeblurImgB, width, height));
#else
		printf(".");
#endif
	}

	delete [] DeltaImgR;
	delete [] DeltaImgG;
	delete [] DeltaImgB;
	delete [] BilateralRegImgR;
	delete [] BilateralRegImgG;
	delete [] BilateralRegImgB;
}

void ProjectiveMotionRL::ProjectiveMotionRLDeblurBilateralLapReg(float* BlurImg, int iwidth, int iheight, float* DeblurImg, int width, int height, int Niter, bool bPoisson, float lambda){
	int i, t = 1;
	//Parameters are set according to Levin et al Siggraph'07
	float powD = 0.8f, noiseVar = 0.0005f, epilson = t / 255.0f;
	float minWeight = exp(-pow(epilson,powD) / noiseVar) * pow(epilson,powD-1.0f);

	//Bilateral Laplician Regularization
	for(i = 0; i <= t; i++){
		BilateralTable[i] = 1.0f;
	}
	for(i = t+1; i < 256; i++){
		BilateralTable[i] = (exp(-pow(i/255.0f,powD) / noiseVar) * pow(i/255.0f,powD-1.0f) ) / minWeight;
	}

	ProjectiveMotionRLDeblurBilateralReg(BlurImg, iwidth, iheight, DeblurImg, width, height, Niter, bPoisson, lambda);
	
	//Restore the original table
	for(i = 0; i < 256; i++){
		BilateralTable[i] = exp(-i*i / (noiseVar * 65025.0f) ) ;
	}	
}

void ProjectiveMotionRL::ProjectiveMotionRLDeblurBilateralLapReg(float* BlurImgR, float* BlurImgG, float* BlurImgB, int iwidth, int iheight, float* DeblurImgR, float* DeblurImgG, float* DeblurImgB, int width, int height, int Niter, bool bPoisson, float lambda){
	int i, t = 1;
	//Parameters are set according to Levin et al Siggraph'07
	float powD = 0.8f, noiseVar = 0.005f, epilson = t / 255.0f;
	float minWeight = exp(-pow(epilson,powD) / noiseVar) * pow(epilson,powD-1.0f);

	//Bilateral Laplician Regularization
	for(i = 0; i <= t; i++){
		BilateralTable[i] = 1.0f;
	}
	for(i = t+1; i < 256; i++){
		BilateralTable[i] = (exp(-pow(i/255.0f,powD) / noiseVar) * pow(i/255.0f,powD-1.0f) ) / minWeight;
	}

	ProjectiveMotionRLDeblurBilateralReg(BlurImgR, BlurImgG, BlurImgB, iwidth, iheight, DeblurImgR, DeblurImgG, DeblurImgB, width, height, Niter, bPoisson, lambda);
	
	//Restore the original table
	for(i = 0; i < 256; i++){
		BilateralTable[i] = exp(-i*i / (noiseVar * 65025.0f) ) ;
	}
}

void ProjectiveMotionRL::ComputeGradientXImage(float* Img, int width, int height, float* DxImg, bool bflag){
	int x,y,index;
	for(y = 0, index = 0; y < height; y++){
		for(x = 0; x < width; x++, index++){
			if(bflag){
				if(x > 0){
					DxImg[index] = Img[index] - Img[index-1];
				}else{
					DxImg[index] = 0;
				}
			}else{
				if(x < width-1){
					DxImg[index] = Img[index] - Img[index+1];
				}else{
					DxImg[index] = 0;
				}
			}
		}
	}
}

void ProjectiveMotionRL::ComputeGradientYImage(float* Img, int width, int height, float* DyImg, bool bflag){
	int x,y,index;
	for(y = 0, index = 0; y < height; y++){
		for(x = 0; x < width; x++, index++){
			if(bflag){
				if(y > 0){
					DyImg[index] = Img[index] - Img[index-width];
				}else{
					DyImg[index] = 0;
				}
			}else{
				if(y < height-1){
					DyImg[index] = Img[index] - Img[index+width];
				}else{
					DyImg[index] = 0;
				}
			}
		}
	}
}

void ProjectiveMotionRL::ComputeGradientImage(float* Img, int width, int height, float* DxImg, float* DyImg, bool bflag){
	int x,y,index;
	for(y = 0, index = 0; y < height; y++){
		for(x = 0; x < width; x++, index++){
			if(bflag){
				if(x > 0){
					DxImg[index] = Img[index] - Img[index-1];
				}else{
					DxImg[index] = 0;
				}
				if(y > 0){
					DyImg[index] = Img[index] - Img[index-width];
				}else{
					DyImg[index] = 0;
				}
			}else{
				if(x < width-1){
					DxImg[index] = Img[index] - Img[index+1];
				}else{
					DxImg[index] = 0;
				}
				if(y < height-1){
					DyImg[index] = Img[index] - Img[index+width];
				}else{
					DyImg[index] = 0;
				}
			}
		}
	}
}

void ProjectiveMotionRL::ComputeBilaterRegImage(float* Img, int width, int height, float* BRImg){
	//Sigma approximately equal to 1
	float GauFilter[5][5] = {{0.01f,0.02f,0.03f,0.02f,0.01f},
							 {0.02f,0.03f,0.04f,0.03f,0.02f},
							 {0.03f,0.04f,0.05f,0.04f,0.03f},
							 {0.02f,0.03f,0.04f,0.03f,0.02f},
							 {0.01f,0.02f,0.03f,0.02f,0.01f}};
	int x,y,index,xx,yy,iindex, iiindex;
	memset(BRImg, 0, width*height*sizeof(float));

	//Compute the long distance 2nd derivative image weighted by Bilateral filter
	for(y = 0, index = 0; y < height; y++){
		for(x = 0; x < width; x++, index++){
			for(xx = -2; xx <=2; xx++){
				if( x+xx >= 0 && x+xx < width && x-xx >= 0 && x-xx < width ){
					for(yy = -2; yy <=2; yy++){
						if( y+yy >= 0 && y+yy < height && y-yy >= 0 && y-yy < height ){
							iindex = (y+yy) * width + (x+xx);
							iiindex = (y-yy) * width + (x-xx);
							BRImg[index] += GauFilter[xx+2][yy+2] * 0.5f*(BilateralTable[(int)(fabs(Img[iindex]-Img[index])*255.0f)]+BilateralTable[(int)(fabs(Img[iiindex]-Img[index])*255.0f)]) * (2*Img[index] - Img[iindex]-Img[iiindex]);
						}
					}
				}
			}
		}
	}
}

/*
void ProjectiveMotionRL::ProjectiveMotionRLDeblurMultiScale(float* BlurImg, int iwidth, int iheight, float* DeblurImg, int width, int height, int Niter, int Nscale, bool bPoisson){
	int i,iscale,x,y,index, itr;
	float* InputWeight = NULL;
	float HFactor[NumSamples];
	float ScaleFactor = sqrt(2.0f);
	for(i = 0; i < NumSamples; i++){
		HFactor[i] = Hmatrix[i].Hmatrix[2][2];
	}

	float* DeltaImg = new float[width*height];

	ClearBuffer();
	SetBuffer(width, height);

	for(iscale = Nscale-1; iscale >= 0; iscale--){
		
		float powfactor = pow(ScaleFactor, iscale);
		int bwidth = (int)(iwidth/powfactor), bheight = (int)(iheight/powfactor);
		printf("Level %d: %d %d\n", iscale, bwidth, bheight);
		float bfactw = (float)(iwidth-1)/(float)(bwidth-1), bfacth = (float)(iheight-1)/(float)(bheight-1);
		for(i = 0; i < NumSamples; i++){
			Hmatrix[i].Hmatrix[2][2] = HFactor[i] * powfactor;
			Hmatrix[i].MatrixInverse(Hmatrix[i].Hmatrix, IHmatrix[i].Hmatrix);
		}

		for(itr = 0; itr < Niter; itr++){
			printf("%d\n", itr);
			GenerateMotionBlurImg(DeblurImg, InputWeight, width, height, BlurImgBuffer, BlurWeightBuffer, bwidth, bheight, true);
			for(y = 0, index = 0; y < bheight; y++){
				for(x = 0; x < bwidth; x++, index++){
					float bvalue = ReturnInterpolatedValueFast(x*bfactw, y*bfacth, BlurImg, iwidth, iheight);
					if(bPoisson){
						if( BlurImgBuffer[index] > 0.0001f){
							DeltaImg[index] = bvalue / BlurImgBuffer[index];
						}else{
							DeltaImg[index] = bvalue / 0.0001f;
						}
					}else{
						DeltaImg[index] = bvalue - BlurImgBuffer[index];
					}
				}
			}
			GenerateMotionBlurImg(DeltaImg, BlurWeightBuffer, bwidth, bheight, ErrorImgBuffer, ErrorWeightBuffer, width, height, false);
			for(y = 0, index = 0; y < height; y++){
				for(x = 0; x < width; x++, index++){
					if(bPoisson){
						DeblurImg[index] *= ErrorImgBuffer[index];
					}else{
						DeblurImg[index] += ErrorImgBuffer[index];
					}
					if(DeblurImg[index] < 0) DeblurImg[index] = 0;
					if(DeblurImg[index] > 1) DeblurImg[index] = 1;
				}
			}
		}
	}

	delete [] DeltaImg;
}*/