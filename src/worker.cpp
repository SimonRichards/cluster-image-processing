#include <iostream>
#include <ctime>
#include "deconvfilter.h"
#include "serverconnection.h"
#include "image.h"
#define DEBUG

using namespace std;

#ifdef DEBUG
void info(string msg) {
    cout << msg << endl;
}
#else
#define info(x)
#endif

int main(int argc, char* argv[]){

    DeconvFilter filter;
    ServerConnection connection;
    Image *image;
    bool received = false, processed = false;

    if (!connection.connect()) {
        cerr << "Failed to connect" << endl;
        return -1;
    }

#pragma omp parallel sections
    {
#pragma omp section
        {
            info("retrieving image");
            image = connection.retrieveImage();
#pragma omp flush
            received = true;
            processed = false;
#pragma omp flush(received,processed)
            while (!processed) {
#pragma omp flush(processed)
                sleep(10);
            }
#pragma omp flush
            info("sending image");
            connection.sendImage(filter.getImage()); // assuming we process the image in place
        }

#pragma omp section
        {
#pragma omp flush
            while (!received) {
#pragma omp flush(received)
                sleep(10);
            }
            received = false;
            info("processing image");
            filter.process(image);
            processed = true;
#pragma omp flush(processed)
        }
    }

    return 0;
}
