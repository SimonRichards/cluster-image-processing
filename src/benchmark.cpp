#include <ctime>
#include "./debug.h"
#include "./deconvfilter.h"
#include "./imagequeue.h"

// ------- Settings ------ //
#define WIDTH 1024
#define HEIGHT 1024
#define NITER 10

#define BUFFER_SIZE WIDTH*HEIGHT
int benchmark(int argc, char* argv[]) {
    static double buffer[BUFFER_SIZE];
    int psfWidth, psfHeight;

    unsigned int niter = argc > 1 ? atoi(argv[1]) : 10;

    // Set up producer
    ImageQueue images(buffer, BUFFER_SIZE, "../images", 1);
    double* psf = ImageQueue::getPsf(&psfWidth, &psfHeight);

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
        images.pop(0);
        imageTimer.begin();
        filter.process();
        FPRINT("Finished %d iterations on an image in %f seconds", niter, imageTimer.getElapsed());
        images.save(0);
    }
    FPRINT("Finished %d image(s) in %f seconds", numImages, mainTimer.getElapsed());
    return 0;
}
