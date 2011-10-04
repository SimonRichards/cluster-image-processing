#pragma once

#include "image.h"

class DeconvFilter {
    private:

    public:
    DeconvFilter();
    ~DeconvFilter();
    
    void process(Image * image);
    Image* getImage();
};
