#include "imagequeue.h"
#include "fitsio.h"
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "debug.h"

using std::string;


// Choice of hard coded psf is made in main file,
// A better way of doing this may be implemented later
static double hardCodedPsf[] = {

#if KERNEL == SMALL_KERNEL
0,1,0,
1,1,1,
0,1,0};
#define KERNEL_WIDTH  3
#define KERNEL_HEIGHT 3

#elif KERNEL == GAUSSIAN_KERNEL
0.00000067 , 0.00002292 , 0.00019117 , 0.00038771 , 0.00019117 , 0.00002292 , 0.00000067,
0.00002292 , 0.00078633 , 0.00655965 , 0.01330373 , 0.00655965 , 0.00078633 , 0.00002292,
0.00019117 , 0.00655965 , 0.05472157 , 0.11098164 , 0.05472157 , 0.00655965 , 0.00019117,
0.00038771 , 0.01330373 , 0.11098164 , 0.22508352 , 0.11098164 , 0.01330373 , 0.00038771,
0.00019117 , 0.00655965 , 0.05472157 , 0.11098164 , 0.05472157 , 0.00655965 , 0.00019117,
0.00002292 , 0.00078633 , 0.00655965 , 0.01330373 , 0.00655965 , 0.00078633 , 0.00002292,
0.00000067 , 0.00002292 , 0.00019117 , 0.00038771 , 0.00019117 , 0.00002292 , 0.00000067};
#define KERNEL_WIDTH  7
#define KERNEL_HEIGHT 7

#elif KERNEL == MOTION_KERNEL
0.000817774, 0.0286433,  0.235018,  0.471041,  0.235018, 0.0286433, 0.000817774};
#define KERNEL_WIDTH  7
#define KERNEL_HEIGHT 1
#endif

/**
 * Grabs all the *.fits file names from imagesDir and pushes them onto a stack
 * ready for reading into the given buffer later.
 */
ImageQueue::ImageQueue(double* buffer, int size, string imagesDir, int numWorkers) {
    image = buffer;
    bufferSize = size;
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
}

/**
 * @param width the output psf width
 * @param height the output psf height
 * @return a pointer to the start of the psf data
 */
double* ImageQueue::getPsf(int* width, int* height) {
    *width = KERNEL_WIDTH;
    *height = KERNEL_HEIGHT;
    return hardCodedPsf;
}

/**
 * Loads the next image from the stack off the hard disk and into the buffer.
 */
void ImageQueue::pop(int id) {
    fitsfile *fptr;
    int status = 0,  nfound, anynull;
    long naxes[2], fpixel, npixels;
    string name = files.top();
    string outFile = name;
    outFile.insert(name.length()-5, "-fixed");
    outFile.insert(outFile.find_last_of('/')+1,"out/");
    outFiles[id] = outFile;
    if ( fits_open_file(&fptr, name.c_str(), READONLY, &status) )
        FERROR("Error opening file: %s code: %d", name.c_str(), status);
    files.pop();

    fits_read_keys_lng(fptr, "NAXIS", 1, 2, naxes, &nfound, &status);
    if (status) FERROR("Error reading file data: %s code: %d", name.c_str(), status);

    npixels  = naxes[0] * naxes[1];
    fpixel   = 1;
    float nullval  = 0;

    fits_read_img(fptr, TDOUBLE, fpixel, npixels, &nullval, image, &anynull, &status);
    if (status) FERROR("Error reading file data: %s code: %d", name.c_str(), status);

    fits_close_file(fptr, &status);
    if (status)
        FERROR("Error closing file data: %s code: %d", name.c_str(), status)
    else
        FPRINT("Read file %s size = %li", name.c_str(), npixels)
}

/**
 * Saves the buffer to disk, file name taken from last image loaded.
 */
void ImageQueue::save(int id) {
    fitsfile *fptr;
    int status = 0;
    long fpixel = 1;

    string outFile = outFiles[id];
    remove(outFile.c_str());

    if ( fits_create_file(&fptr, outFile.c_str(), &status) )
        FERROR("Error creating file: %s code: %d", outFile.c_str(), status);

    long naxes[] = {1024,1024};

    if (fits_create_img(fptr, DOUBLE_IMG, 2, naxes, &status))
        FERROR("Error writing header: %s code: %d", outFile.c_str(), status);

    fits_write_img(fptr, TDOUBLE, fpixel, bufferSize, image, &status);
    if (status) FERROR("Error writing file data: %s code: %d", outFile.c_str(), status);

    fits_close_file(fptr, &status);
    if (status)
        FERROR("Error closing file: %s code: %d", outFile.c_str(), status)
    else
        FPRINT("Wrote file %s", outFile.c_str())

}

/**
 * @return the number of images remaining.
 */
uint32_t ImageQueue::remaining() {
    return files.size();
}



































    // Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space// Allocate space}
