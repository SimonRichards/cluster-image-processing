#include "image.h"
#include "serverconnection.h"
#include <mpi.h>

ServerConnection::ServerConnection() {

}

ServerConnection::~ServerConnection() {

}

bool ServerConnection::connect() {

    return true;
}

void ServerConnection::sendAndReceiveImage(Image* imageToSend) {
 
}

Image* ServerConnection::retrieveImage() {

}

bool ServerConnection::imageReady() {
    return true;
}
