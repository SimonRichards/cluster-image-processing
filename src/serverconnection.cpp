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

void ServerConnection::sendImage(Image* image) {

}

Image* ServerConnection::retrieveImage() {

}
