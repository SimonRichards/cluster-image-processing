#ifndef __IMAGE_RESIZE_H__
#define __IMAGE_RESIZE_H__

extern inline void ImResize(float* Img, int width, int height, float* Rimg, int Rwidth, int Rheight);
extern inline void ImChopping(float* Img, int width, int height, float* Rimg, int Rwidth, int Rheight);
extern inline void ImChopping(float* Img, int width, int height, float* Rimg, int Rwidth, int Rheight, int CenterX, int CenterY);

#endif