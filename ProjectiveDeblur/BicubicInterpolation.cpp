#include "BicubicInterpolation.h"

inline void bicubicweight(float x, float y, float* w){
	float ww;

	float ix = x - (int)(x), iy = y - (int)(y);
	if(x >=0 && y >= 0 ){
		w[0] = (1-ix)*(1-iy);
		w[1] = (1-ix)*(iy);
		w[2] = (ix)*(1-iy);
		w[3] = (ix)*(iy);
		ww = w[0]+w[1]+w[2]+w[3];
	}else if( x >= 0){
		w[0] = (1-ix)*(iy+1);
		w[1] = (1-ix)*(-iy);
		w[2] = (ix)*(iy+1);
		w[3] = (ix)*(-iy);
		ww = w[0]+w[1]+w[2]+w[3];
	}else if( y >= 0){
		w[0] = (ix+1 )*(1-iy);
		w[1] = (ix+1 )*(iy);
		w[2] = (-ix)*(1-iy);
		w[3] = (-ix)*(iy);
		ww = w[0]+w[1]+w[2]+w[3];
	}else{
		w[0] = (ix+1 )*(iy+1);
		w[1] = (ix+1 )*(-iy);
		w[2] = (-ix)*(iy+1);
		w[3] = (-ix)*(-iy);
		ww = w[0]+w[1]+w[2]+w[3];
	}
	
	w[0] /= ww; w[1] /= ww; w[2] /= ww; w[3] /= ww;
}

inline void bicubicweightFast(float x, float y, float* w){
	float ix = x - (int)(x), iy = y - (int)(y);
	w[1] = (1.0f-ix)*iy;
	w[2] = ix*(1.0f-iy);
	w[3] = ix*iy;
	w[0] = 1.0f - w[1] - w[2] - w[3];
}

inline float ReturnInterpolatedValue(float x, float y, float* img, int width, int height){
	float w[4], value;
	int ix = (int)(x), iy = (int)(y);
	int index = iy * width + ix;
	bicubicweight(x,y,w);

	value = img[index]			*w[0] + 
			img[index+width]	*w[1] + 
			img[index+1]		*w[2] + 
			img[index+width+1]	*w[3];

	return value;
}

inline float ReturnInterpolatedValueFast(float x, float y, float* img, int width, int height){
	float w[4];
	int ix = (int)(x), iy = (int)(y);
	int index = iy * width + ix;
	float fx = x - ix, fy = y - iy;
	w[1] = (1.0f-fx)*fy;
	w[2] = fx*(1.0f-fy);
	w[3] = fx*fy;
	w[0] = 1.0f - w[1] - w[2] - w[3];

	return img[index]			*w[0] + 
			img[index+width]	*w[1] + 
			img[index+1]		*w[2] + 
			img[index+width+1]	*w[3];
}

inline void ReturnInterpolatedValueFast(float x, float y, float* Rimg, float* Gimg, float* Bimg, int width, int height, float& R, float& G, float& B){
	float w[4];
	int ix = (int)(x), iy = (int)(y);
	int index = iy * width + ix;
	float fx = x - ix, fy = y - iy;
	w[1] = (1.0f-fx)*fy;
	w[2] = fx*(1.0f-fy);
	w[3] = fx*fy;
	w[0] = 1.0f - w[1] - w[2] - w[3];

	R = Rimg[index]			*w[0] + 
		Rimg[index+width]	*w[1] + 
		Rimg[index+1]		*w[2] + 
		Rimg[index+width+1]	*w[3];
	G = Gimg[index]			*w[0] + 
		Gimg[index+width]	*w[1] + 
		Gimg[index+1]		*w[2] + 
		Gimg[index+width+1]	*w[3];
	B = Bimg[index]			*w[0] + 
		Bimg[index+width]	*w[1] + 
		Bimg[index+1]		*w[2] + 
		Bimg[index+width+1]	*w[3];
}