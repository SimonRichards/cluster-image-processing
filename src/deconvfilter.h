#pragma once

#include "imagequeue.h"
#include <fftw3.h>
#include <vector>

class DeconvFilter {
    private:
        int _width, _height, _psfWidth, _psfHeight;
        unsigned int _niter;
        unsigned long _size;
        double* _buffer;

#ifndef USE_FFT
        double *_psf, *orig, *img, *scratch, *scratch2;
        void convolve(double* result, double* input); // with _psf is implicit
        void divide(double* quotient, double* dividend, double* divisor);
        void multiply(double* product, double* factorA, double* factorB);
        void scale(double* product, double scalar);
        void offset(double* product, double offset);
        void saturate(double *image);
        void convolveCore(double* result, double* input, int y);

        int psfStartOffset;
#else
        fftw_complex *in, *scratch, *orig, *fftPsf, *conjFftPsf;
        fftw_plan fftScratch;
        fftw_plan ifftScratch;
        fftw_plan fftInToScratch;

        void multVec(fftw_complex* lval, fftw_complex* a, fftw_complex* b, int n, double *scale);
        void  divVec(fftw_complex* lval, fftw_complex* a, fftw_complex* b, int n);
        void centrePsf(
                fftw_complex* mat, double* input, int width, int height,
                int psfWidth, int psfHeight);
#endif


    public:
        DeconvFilter(
        int width, int height, unsigned int niter,
        double* psf, int psfWidth, int psfHeight, double* buffer);
        ~DeconvFilter();

        void process();
};
