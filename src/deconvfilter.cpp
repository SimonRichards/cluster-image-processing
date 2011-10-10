#include "deconvfilter.h"
#include <iostream>
#include "debug.h"
#include <cstdio>
#include <cstring>
//#include <omp.h>
using namespace std;

#define EPS 0.000001
#ifndef USE_FFT // small kernel size so use manual convolution

void normalise(double* data, size_t length) {
    unsigned int i;
    double sum = 0;
    for (i = 0; i < length; i++)
        sum += data[i];
    for (i = 0; i < length; i++)
        data[i] /= sum;
}

// Allocates scratch space for the algorithm and stores parameters.
DeconvFilter::DeconvFilter(
        int width, int height, unsigned int niter,
        double* inputPsf, int psfWidth, int psfHeight, double* buffer) :
    _width(width),
    _height(height),
    _psfWidth(psfWidth),
    _psfHeight(psfHeight),
    _niter(niter),
    _size(width*height),
    _buffer(buffer),
    _psf(inputPsf),
    orig(buffer) {
    size_t psfSize = psfWidth * psfHeight;
    psfStartOffset = psfSize / 2;
    normalise(_psf, psfSize);
    img=new double[_size];
    scratch=new double[_size];
    scratch2=new double[_size];
}

// Frees space before destruction.
DeconvFilter::~DeconvFilter() {
    delete[] img;
    delete[] scratch;
}

// result = input*_psf
void DeconvFilter::convolve(double* result, double* input) {
    int x, y, px, py, pIndex;
    long long index = 0;
    int pxOffset = _psfWidth/2;
    int pyOffset = _psfHeight/2;
    for (x = 0; x < _width; x++) {
        for (y = 0; y < _height; y++, index++) {
            pIndex = -psfStartOffset;
            result[index] = 0;
            for (px = 0; px < _psfWidth; px++) {
                for (py = 0; py < _psfHeight; py++, pIndex++) {
                    int absX = x - pxOffset + px;
                    if (
                            absX <= 0 ||
                            absX >= _width
                       ) break; // whole column is pointless, save some time
                    int absY = y - pyOffset + py;
                    if (
                            absY > 0 &&
                            absY < _height
                       )
                        result[index] += _psf[pIndex+psfStartOffset] * input[index + pIndex+psfStartOffset];
                }
            }
        }
    }
}

// quotient[i] = dividend[i] / divisor[i] forall i
void DeconvFilter::divide(double* quotient, double* dividend, double* divisor) {
    for (unsigned int i = 0; i < _size; i++)
        quotient[i] = dividend[i] / divisor[i];
}

// product[i] = factorA[i] * factorB[i] forall i
void DeconvFilter::multiply(double* product, double* factorA, double* factorB) {
    for (unsigned int i = 0; i < _size; i++)
        product[i] = factorA[i] * factorB[i];
}

// product[i] = product[i] * scalar forall i
void DeconvFilter::scale(double* product, double scalar) {
    for (unsigned int i = 0; i < _size; i++)
        product[i] *= scalar;
}

// product[i] = product[i] + offset forall i
void DeconvFilter::scale(double* product, double scalar) {
    for (unsigned int i = 0; i < _size; i++)
        product[i] += offset;
}

void minMax(double* buffer, long long size) {
    double min = 1e9, max = -1e9;
    for (long long i = 0; i < size; i++) {
        if (buffer[i] > max) max = buffer[i];
        if (buffer[i] < min) min = buffer[i];
    }
    FPRINT("Min val = %e, Max val = %e", min, max);
}

/**
 * Performs n iterations of the Richardson-Lucy deconvolution algorithm
 * without using fast fourier transform, useful when the psf kernel size is
 * small.
 */
void DeconvFilter::process() {
    unsigned int iter;
    memcpy(img, _buffer, _size*sizeof(double));
    memcpy(orig, _buffer, _size*sizeof(double));

    for (iter = 0; iter < _niter; iter++) {
        //img_i+1 = img_i.((orig/(p*img_i))*p) where * is the convolution operator
        convolve(scratch, img);
        divide(scratch, orig, scratch);
        convolve(scratch2, scratch);
        multiply(img, img, scratch2);
        scale(img, -0.5);
    }
    memcpy(_buffer, img, _size*sizeof(double));
}

#else //relatively large kernel then use the A*B=ifft(fft(A).*fft(B)) identity

/**
 * Returns an object ready to start filtering
 * images of the given size with the input psf.
 * All scratch space is preallocated for efficiency
 *
 * Will attempt to load FFTW wisdom from file,
 * and will create it otherwise causing the
 * first run to be slow.
 */
DeconvFilter::DeconvFilter(
        int width, int height, unsigned int niter,
        double* inputPsf, int psfWidth, int psfHeight, double* buffer) :
_width(width), _height(height), _niter(niter), _buffer(buffer){

    // Initiate the multithreaded fftw lib. The second line causes
    // a segfault during planning unfortunately so plans are single
    // threaded for now.
    //fftw_init_threads();
    //fftw_plan_with_nthreads(2);//omp_get_num_threads());

    // Allocate space
    _size = width*height;
    in      = fftw_alloc_complex(_size);
    scratch = fftw_alloc_complex(_size);
    orig    = fftw_alloc_complex(_size);
    fftPsf     = fftw_alloc_complex(_size);
    conjFftPsf = fftw_alloc_complex(_size);

    // Load wisdom from file, exploring options is very time consuming
    // so this saves a lot of effort.
    int imported = fftw_import_wisdom_from_filename("wisdom");
    const char* msg = imported ? "Wisdom imported" : "Wisdom not found";
    PRINT(msg);


    // Create fft plans
    fftScratch = fftw_plan_dft_2d(
            width, height, scratch, scratch, FFTW_FORWARD, FFTW_PATIENT);
    ifftScratch = fftw_plan_dft_2d(
            width, height, scratch, scratch, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftInToScratch = fftw_plan_dft_2d(
            width, height, in, scratch, FFTW_FORWARD, FFTW_ESTIMATE);

    centrePsf(fftPsf, inputPsf, width, height, psfWidth, psfHeight);
    fftw_plan fft = fftw_plan_dft_2d(
            width, height, fftPsf, fftPsf, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(fft);

    // If we didn't load from a file, then save to one for the next guy
    if (!imported) fftw_export_wisdom_to_filename("wisdom");

    // Save the PSF's complex conjugate
    for (unsigned long i = 0; i < _size; i++) {
        conjFftPsf[i][0] = fftPsf[i][0];
        conjFftPsf[i][1] = -fftPsf[i][0];
    }
}


DeconvFilter::~DeconvFilter() {
    fftw_free(in);
    fftw_free(scratch);
    fftw_free(fftPsf);
    fftw_free(conjFftPsf);
    fftw_destroy_plan(fftScratch);
    fftw_destroy_plan(ifftScratch);
    fftw_destroy_plan(fftInToScratch);
    fftw_cleanup_threads();
}

void DeconvFilter::process() {
    fftw_complex *temp;
    unsigned int i, iter;
    double* scale = NULL;
    for (i = 0; i < _size ; i++) {
        orig[i][0] = in[i][0] = _buffer[i];
        orig[i][1] = in[i][1] = 0.0;
    }
    for (iter = 0; iter < _niter; iter++) {
        fftw_execute(fftInToScratch);
        multVec(scratch, fftPsf, scratch, _size, &scale);
        fftw_execute(ifftScratch);
        divVec(scratch,orig,scratch,_size);
        fftw_execute(fftScratch);
        multVec(scratch, scratch, conjfftPsf, _size, &scale);
        fftw_execute(ifftScratch);
        multVec(in, in, scratch, _size, &scale);
    }
    for (i = 0; i < _size ; i++) {
        _buffer[i] = in[i][0];
    }
}

/**
 * lval = a * b in complex space when n > 0
 * lval = a * b in real    space when n < 0
 * if scale is not null then the results are multiplied by *scale
 */
void DeconvFilter::multVec(fftw_complex* lval, fftw_complex* a, fftw_complex* b, int n, double *scale) {

    double tmp;
    if (--n>=0) {
        if (scale) {
            do {
                tmp = a[n][0]*b[n][0] - a[n][1]*b[n][1];
                lval[n][1] = a[n][0]*b[n][1] + a[n][1]*b[n][0];
                lval[n][0] = tmp;
            } while (--n > -1);
        } else {
            double s = *scale;
            do {
                tmp = (a[n][0]*b[n][0] - a[n][1]*b[n][1]) * s;
                lval[n][1] = (a[n][0]*b[n][1] + a[n][1]*b[n][0]) * s;
                lval[n][0] = tmp;
            } while (--n > -1);
        }
    } else {
        n = n*-1 - 1;
        if (scale) {
            do
                lval[n][1] = a[n][0]*b[n][0];
            while (--n > -1);
        } else {
            double s = *scale;
            do
                lval[n][0] = (a[n][0]*b[n][0]) * s;
            while (--n > -1);
        }
    }
}

/**
 * lval.re = a.re / b.re
 * lval.im = a.im / b.im
 * On an a per element basis.
 */
void DeconvFilter::divVec(fftw_complex* lval, fftw_complex* a, fftw_complex* b, int n) {
    for (int i = 0; i < n; i++) {
        lval[i][0] = a[i][0]/(b[i][0]+EPS);
        lval[i][1] = a[i][1]/(b[i][1]+EPS);
    }
}

/**
 * Loads a psf into mat centred at the top left corner with the
 * rest of mat zeroed out.
 */
void DeconvFilter::centrePsf(
        fftw_complex* mat, double* input, int width, int height,
        int psfWidth, int psfHeight) {

    int i, j, index = 0;
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++, index++) {
            mat[index][0] = 0.0;
            mat[index][1] = 0.0;
        }
    }

    index = 0;
    for (i = psfWidth/2; i < psfWidth; i++) {
        for (j = psfHeight/2; j < psfHeight; j++, index++) {
            mat[index][0] = input[i*psfWidth+j];
        }
        index += width - psfWidth/2;
    }
}

#endif



