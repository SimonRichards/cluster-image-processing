#pragma once
#include <string>
#include <stack>
#include <map>

class ImageQueue {
    private:
        double* image;
        long long bufferSize;
        std::stack<std::string> files;
        std::map<int,std::string> outFiles;

    public:
        ImageQueue(double* buffer, int size, std::string imagesDir, int numWorkers);
        ~ImageQueue();
        void pop(int id);
        void save(int id);
        static double* getPsf(int* width, int* height);
        uint32_t remaining();
};
