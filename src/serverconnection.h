#pragma once

#include "image.h"

class ServerConnection {
    private:

    public:
        ServerConnection();
        ~ServerConnection();

        bool imageReady();
        bool connect();
        void sendAndReceiveImage(Image* imageToSend);
        Image* retrieveImage();
};
