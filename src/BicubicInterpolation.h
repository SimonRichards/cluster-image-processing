#ifndef __BICUBIC_INTERPOLATION_H__
#define __BICUBIC_INTERPOLATION_H__

void bicubicweight(float x, float y, float* w);
void bicubicweightFast(float x, float y, float* w);
float ReturnInterpolatedValue(float x, float y, float* img, int width, int height);
float ReturnInterpolatedValueFast(float x, float y, float* img, int width, int height);
void ReturnInterpolatedValueFast(float x, float y, float* Rimg, float* Gimg, float* Bimg, int width, int height, float& R, float& G, float& B);

#endif
