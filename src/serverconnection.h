#pragma once

#include "image.h"

class ServerConnection {
    private:

    public:
        ServerConnection();
        ~ServerConnection();

        bool connect();
        void sendImage(Image* image);
        Image* retrieveImage();
};
