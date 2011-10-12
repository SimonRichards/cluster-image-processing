#include <ctime>
#include <mpi.h>
#include "./debug.h"
#include "./deconvfilter.h"
#include "./imagequeue.h"

// ----- Explicit declaration of the non-mpi version of the code ----- //
void benchmark(int argc, char* argv[]);

// ------- Enumerated values ------- //
enum {
    PSF,
    IMG,
    END
};

// ------- Settings ------ //
#define WIDTH 1024
#define HEIGHT 1024
#define NITER 10  // Number of iterations per image

#define BUFFER_SIZE WIDTH*HEIGHT

int main(int argc, char* argv[]) {
#if 1
    benchmark(argc, argv);
#else
    // mpi setup
    int numProcs;
    int rank, flag;
    int done = 0;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // create a buffer for both worker and controller
    static double buffer[BUFFER_SIZE];
    unsigned int niter = argc > 1 ? atoi(argv[1]) : NITER;

    // Setting up the PSF (statically)
    int psfWidth, psfHeight;
    double* psf = ImageQueue::getPsf(&psfWidth, &psfHeight);

    // ---------- CONTROLLER NODE ---------- //
    if (rank == 0) {
        // Set up producer
        ImageQueue images(buffer, BUFFER_SIZE, "../images", numProcs);

        // Print out some details
        int numImages = images.remaining();
        FPRINT("Starting %d iteration(s) on %d image(s)", niter, numImages);
        PerfTimer mainTimer;
        mainTimer.begin();

        int toSend = (unsigned int)numProcs < images.remaining() ? numProcs : images.remaining();
        for (int i = 0; i < toSend; i++) {
            images.pop(i);
            MPI_Send(buffer, BUFFER_SIZE, MPI_DOUBLE, i, IMG, MPI_COMM_WORLD);
        }

        while (images.remaining() > 0) {
            for (int i = 0; i < numProcs; i++) {

                // If an image is received then save it and send the next one
                MPI_Iprobe(i, IMG, MPI_COMM_WORLD, &flag, &status);
                if (flag) {
                    MPI_Recv(buffer, BUFFER_SIZE, MPI_DOUBLE, i, IMG, MPI_COMM_WORLD, &status);
                    images.save(i);
                    images.pop(i);
                    MPI_Send(buffer, BUFFER_SIZE, MPI_DOUBLE, i, IMG, MPI_COMM_WORLD);
                }
            }
        }

        for (int i = 0; i < numProcs; i++) {
            MPI_Send(&done, 1, MPI_INT, i, END, MPI_COMM_WORLD);
        }
        FPRINT("Finished %d image(s) in %f seconds", numImages, mainTimer.getElapsed());
    }

    // ---------- WORKER NODE ---------- //
    else { // worker thread
        // Set up consumer
        DeconvFilter filter(WIDTH, HEIGHT, niter, psf, psfWidth, psfHeight, buffer);
        bool running = true;
        PRINT("Worker thread initialised.");

        while (running) {
            MPI_Iprobe(0, IMG, MPI_COMM_WORLD, &flag, &status);
            if (flag) { // New image
                MPI_Recv(buffer, BUFFER_SIZE, MPI_DOUBLE, 0, IMG, MPI_COMM_WORLD, &status);
                filter.process();
                MPI_Send(buffer, BUFFER_SIZE, MPI_DOUBLE, 0, IMG, MPI_COMM_WORLD);
            }

            MPI_Iprobe(0, END, MPI_COMM_WORLD, &flag, &status);
            if (flag) { // Execution finished
                MPI_Recv(&done, 1, MPI_INT, 0, END, MPI_COMM_WORLD, &status);
                running = false;
            }
        }
        PRINT("Worker thread finished.");
    }

    MPI_Finalize();
#endif
    return 0;
}

