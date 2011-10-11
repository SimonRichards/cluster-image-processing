#pragma once
#include <string>
#include <stack>

class ImageQueue {
    private:
        double* image;
        long long bufferSize;
        std::stack<std::string> files;
        std::string outFile;

    public:
        ImageQueue(double* buffer, long long size, std::string imagesDir);
        ~ImageQueue();
        void pop();
        void save();
        double* getPsf(int* width, int* height);
        uint32_t remaining();
};
