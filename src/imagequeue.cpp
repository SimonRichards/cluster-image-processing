#include "imagequeue.h"
#include "fitsio.h"
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "debug.h"

using namespace std;

ImageQueue::ImageQueue(double* buffer, long size, string imagesDir) {
    image = buffer;
    size  = bufferSize;
    // Grab all the *.fits file names from the 'images' folder
    imagesDir.append("/");
    DIR *dp = opendir (imagesDir.c_str());
    struct dirent *ep;


    if (dp != NULL) {;
        ep = readdir (dp);
        do  {
            string name(ep->d_name);
            if (name.length() > 5 && name.compare(name.size()-5,5, ".fits") == 0) {
                name.insert(0, imagesDir);
                files.push(name);
                cout << name << endl;
            }
            ep = readdir (dp);
        } while (ep);

        closedir (dp);
    } else {
        ERROR("Directory not found");
    }
    switch (files.size()) {
        case 0:
            WARNING("No files found");
            break;
        case 1:
            PRINT("1 file found");
            break;
        default:
            FPRINT("%d files found", files.size());
    }
}


ImageQueue::~ImageQueue() {
    delete []psf;
}

double* ImageQueue::getPsf(int* width, int* height) {
    *width = *height = 10;
    psf = new double[*width * *height];
    return psf;
}

long ImageQueue::pop() {
    fitsfile *fptr;
    int status = 0,  nfound, anynull;
    long naxes[2], fpixel, npixels;

    string name = files.top();
    if ( fits_open_file(&fptr, name.c_str(), READONLY, &status) ) {
        FERROR("Error opening file: %s code: %d", name.c_str(), status);
        return -1;
    }
    files.pop();

    fits_read_keys_lng(fptr, "NAXIS", 1, 2, naxes, &nfound, &status);
    if (status) FERROR("Error reading file data: %s code: %d", name.c_str(), status);

    npixels  = naxes[0] * naxes[1];
    fpixel   = 1;
    float nullval  = 0;

    fits_read_img(fptr, TDOUBLE, fpixel, npixels, &nullval, image, &anynull, &status);
    if (status) FERROR("Error reading file data: %s code: %d", name.c_str(), status);

    fits_close_file(fptr, &status);
    if (status) FERROR("Error closing file data: %s code: %d", name.c_str(), status);

    FPRINT("Read file %s size = %li", name.c_str(), npixels);

    return npixels;
}

bool ImageQueue::remain() {
    return !files.empty();
}
