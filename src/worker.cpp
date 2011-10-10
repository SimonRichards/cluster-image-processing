#include <ctime>
#include "./debug.h"
#include "./deconvfilter.h"
#include "./imagequeue.h"
#define WIDTH 1024
#define HEIGHT 1024
#define NITER 4
#define BUFFER_SIZE WIDTH*HEIGHT


int main(int argc, char* argv[]) {
    static double buffer[BUFFER_SIZE];
    int psfWidth, psfHeight;
    time_t last;

    unsigned int niter = argc > 1 ? atoi(argv[1]) : NITER;

    ImageQueue images(buffer, BUFFER_SIZE, "../images");
    double* psf = images.getPsf(&psfWidth, &psfHeight);

    DeconvFilter filter(WIDTH, HEIGHT, niter, psf, psfWidth, psfHeight, buffer);

    while (images.remain()) {
        last = START_TIMER();
        images.pop();
        filter.process();
        images.save();
        PRINT_TIMER("Iterations", last);
    }
    return 0;
}
