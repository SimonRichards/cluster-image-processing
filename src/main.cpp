#include <ctime>
#include <mpi.h>
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

int main(int argc, char* argv[]) {
    // mpi setup
    int numProcs, rank;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // create a buffer for both worker and controller
    static double buffer[BUFFER_SIZE];

    // Start working through images
    if (rank == 0) { // controller thread
        // Set up producer
        int psfWidth, psfHeight;
        ImageQueue images(buffer, BUFFER_SIZE, "../images");
        double* psf = images.getPsf(&psfWidth, &psfHeight);

        // Print out some details
        int numImages = images.remaining();
        FPRINT("Starting %d iteration(s) on %d image(s)", niter, numImages);
        PerfTimer mainTimer;
        mainTimer.begin();

        while (images.remaining() > 0) {
            PerfTimer imageTimer;
            images.pop();
            imageTimer.begin();
            filter.process();
            FPRINT("Finished %d iterations on an image in %f seconds", niter, imageTimer.getElapsed());
            images.save();
        }
        FPRINT("Finished %d image(s) in %f seconds", numImages, mainTimer.getElapsed());

    } else { // worker thread
        // Set up consumer
        unsigned int niter = argc > 1 ? atoi(argv[1]) : NITER;
        DeconvFilter filter(WIDTH, HEIGHT, niter, psf, psfWidth, psfHeight, buffer);


    }
    MPI_Finalize();
    return 0;
}
