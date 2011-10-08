#pragma once
#include <string>
#include <stack>

class ImageQueue {
    private:
        double* image;
        long bufferSize;
        double* psf;
        std::stack<std::string> files;

    public:
        ImageQueue(double* buffer, long size, std::string imagesDir);
        ~ImageQueue();
        long pop();
        double* getPsf(int* width, int* height);
        bool remain();
};
