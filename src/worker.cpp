#include <iostream>
#include <ctime>
#include "debug.h"
#include "deconvfilter.h"
#include "imagequeue.h"
#include "time.h"
#define WIDTH 1024
#define HEIGHT 1024
#define NITER 1
#define BUFFER_SIZE WIDTH*HEIGHT

using namespace std;


int main(int argc, char* argv[]){

    static double buffer[BUFFER_SIZE];
    int psfWidth, psfHeight;
    time_t last;

    cout << BUFFER_SIZE << endl;

    ImageQueue images(buffer, BUFFER_SIZE, "../images");
    double* psf = images.getPsf(&psfWidth, &psfHeight);

    DeconvFilter filter(WIDTH, HEIGHT, NITER, psf, psfWidth, psfHeight, buffer);

    while(images.remain()) {
        last = time(NULL);
        images.pop();
        filter.process();
        images.save();
        cout << "Image took " << time(NULL) - last << " seconds" << endl;
    }
    return 0;
}
