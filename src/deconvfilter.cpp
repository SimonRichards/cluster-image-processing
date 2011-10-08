#include "deconvfilter.h"
#include <iostream>
#include <omp.h>

using namespace std;

DeconvFilter::DeconvFilter(
        int width, int height, unsigned int niter,
        double* inputPsf, int psfWidth, int psfHeight, double* buffer) :
_width(width), _height(height), _niter(niter), _buffer(buffer){

    fftw_init_threads();
    fftw_plan_with_nthreads(4);//omp_get_num_threads());

    long size = width*height;
    in      = fftw_alloc_complex(size);
    scratch = fftw_alloc_complex(size);
    out     = fftw_alloc_complex(size);
    orig    = fftw_alloc_complex(size);
    psf     = fftw_alloc_complex(size);
    conjPsf = fftw_alloc_complex(size);

    fftScratch = fftw_plan_dft_2d(
            width, height, scratch, scratch, FFTW_FORWARD, FFTW_PATIENT);
    ifftScratch = fftw_plan_dft_2d(
            width, height, scratch, scratch, FFTW_BACKWARD, FFTW_PATIENT);
    fftInToScratch = fftw_plan_dft_2d(
            width, height, in, scratch, FFTW_FORWARD, FFTW_PATIENT);

    centrePsf(psf, inputPsf, width, height, psfWidth, psfHeight);
    fftw_plan fft = fftw_plan_dft_2d(
            width, height, psf, psf, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(fft);
    for (unsigned long i = 0; i < _size; i++) {
        conjPsf[i][0] = psf[i][0];
        conjPsf[i][1] = -psf[i][0];
    }
}

DeconvFilter::~DeconvFilter() {
    fftw_free(in);
    fftw_free(scratch);
    fftw_free(out);
    fftw_free(psf);
    fftw_free(conjPsf);
    fftw_destroy_plan(fftScratch);
    fftw_destroy_plan(ifftScratch);
    fftw_destroy_plan(fftInToScratch);
    fftw_cleanup_threads();
}

void DeconvFilter::process() {
    fftw_complex *temp;
    unsigned int i, iter;
    double scale = 1.0;
    for (i = 0; i < _size ; i++) {
        orig[i][0] = in[i][0] = _buffer[i];
        orig[i][1] = in[i][1] = 0.0;
    }
    for (iter = 0; iter < _niter; iter++) {
        fftw_execute(fftInToScratch);
        multVec(scratch, psf, scratch, _size, &scale);
        fftw_execute(ifftScratch);
        divVec(scratch,orig,scratch,_size);
        fftw_execute(fftScratch);
        multVec(scratch, scratch, conjPsf, _size, &scale);
        fftw_execute(ifftScratch);
        multVec(out, in, scratch, _size, &scale);
        temp = in;
        in = out;
        out = temp;
    }
    for (i = 0; i < _size ; i++)
        _buffer[i] = out[i][0];
}

void DeconvFilter::multVec(fftw_complex* lval, fftw_complex* a, fftw_complex* b, int n, double *scale) {
    for (int i = 0; i < n; i++) {
    }
}

void DeconvFilter::divVec(fftw_complex* lval, fftw_complex* a, fftw_complex* b, int n) {
    for (int i = 0; i < n; i++) {
    }
}

void DeconvFilter::centrePsf(
        fftw_complex* mat, double* input, int width, int height,
        int psfWidth, int psfHeight) {

}

