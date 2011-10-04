#include <iostream>
#include <ctime>
#include "debug.h"
#include "deconvfilter.h"
#include "serverconnection.h"
#include "image.h"
#define DEBUG

using namespace std;


int main(int argc, char* argv[]){

    DeconvFilter filter;
    ServerConnection connection;
    Image *image;

    if (!connection.connect()) {
        cerr << "Failed to connect" << endl;
        return -1;
    }

    while (connection.imageReady()) {
        image = connection.retrieveImage();
        filter.process(image);
        connection.sendAndReceiveImage(image);
    }

    return 0;
}
