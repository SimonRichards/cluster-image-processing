#ifndef __BICUBIC_INTERPOLATION_H__
#define __BICUBIC_INTERPOLATION_H__

extern inline void bicubicweight(float x, float y, float* w);
extern inline void bicubicweightFast(float x, float y, float* w);
extern inline float ReturnInterpolatedValue(float x, float y, float* img, int width, int height);
extern inline float ReturnInterpolatedValueFast(float x, float y, float* img, int width, int height);
extern inline void ReturnInterpolatedValueFast(float x, float y, float* Rimg, float* Gimg, float* Bimg, int width, int height, float& R, float& G, float& B);

#endif