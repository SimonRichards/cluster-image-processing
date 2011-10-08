#pragma once

#include "imagequeue.h"
#include <fftw3.h>

class DeconvFilter {
    private:
        int _width;
        int _height;
        unsigned int _niter;
        unsigned long _size;
        double* _buffer;
        fftw_complex *in, *out, *scratch, *orig, *psf, *conjPsf;
        fftw_plan fftScratch;
        fftw_plan ifftScratch;
        fftw_plan fftInToScratch;

        void multVec(fftw_complex* lval, fftw_complex* a, fftw_complex* b, int n, double *scale);
        void  divVec(fftw_complex* lval, fftw_complex* a, fftw_complex* b, int n);
        void centrePsf(
                fftw_complex* mat, double* input, int width, int height,
                int psfWidth, int psfHeight);


    public:
        DeconvFilter(
        int width, int height, unsigned int niter,
        double* psf, int psfWidth, int psfHeight, double* buffer);
        ~DeconvFilter();

        void process();
};
