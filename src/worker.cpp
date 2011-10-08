#include <iostream>
#include <ctime>
#include "debug.h"
#include "deconvfilter.h"
#include "imagequeue.h"
#define WIDTH 1024
#define HEIGHT 1024
#define NITER 1
#define BUFFER_SIZE WIDTH*HEIGHT

using namespace std;


int main(int argc, char* argv[]){

    static double buffer[BUFFER_SIZE];
    double* psf;
    int psfWidth, psfHeight;

    ImageQueue images(buffer, BUFFER_SIZE, "../images");
    psf = images.getPsf(&psfWidth, &psfHeight);

    DeconvFilter filter(WIDTH, HEIGHT, NITER, psf, psfWidth, psfHeight, buffer);

    while(images.remain()) {
        images.pop();
        filter.process();
    }
    return 0;
}
