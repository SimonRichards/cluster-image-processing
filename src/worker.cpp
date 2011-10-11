#include <ctime>
#include "./debug.h"
#include "./deconvfilter.h"
#include "./imagequeue.h"

#define SMALL_KERNEL    1
#define GAUSSIAN_KERNEL 2
#define MOTION_KERNEL   3

// ------- Settings ------ //
#define WIDTH 1024
#define HEIGHT 1024
#define NITER 1  // Number of iterations per image
#define KERNEL 1 // choose from the above

#define BUFFER_SIZE WIDTH*HEIGHT

int worker(int argc, char* argv[]) {
    static double buffer[BUFFER_SIZE];
    int psfWidth, psfHeight;

    unsigned int niter = argc > 1 ? atoi(argv[1]) : NITER;

    // Set up producer
    ImageQueue images(buffer, BUFFER_SIZE, "../images");
    double* psf = images.getPsf(&psfWidth, &psfHeight);

    // Set up consumer
    DeconvFilter filter(WIDTH, HEIGHT, niter, psf, psfWidth, psfHeight, buffer);

    // Print out some details
    int numImages = images.remaining();
    FPRINT("Starting %d iteration(s) on %d image(s)", niter, numImages);
    PerfTimer mainTimer;
    mainTimer.begin();

    // Start working through images
    while (images.remaining() > 0) {
        PerfTimer imageTimer;
        images.pop();
        imageTimer.begin();
        filter.process();
        FPRINT("Finished %d iterations on an image in %f seconds", niter, imageTimer.getElapsed());
        images.save();
    }
    FPRINT("Finished %d image(s) in %f seconds", numImages, mainTimer.getElapsed());
    return 0;
}
