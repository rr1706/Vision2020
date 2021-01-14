// I got a lot of help from this tutorial https://www.youtube.com/watch?v=cNdlrbZSkyQ&t=1142s
#include "headers/WebStream.hpp"

#include <arpa/inet.h>
#include <cstdio>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "opencv2/opencv.hpp"
#include "lua5.3/luasocket.h"

Webstream::Webstream() {
	this->sock = socket( AF_INET, SOCK_STREAM, 0 );
	if( this->sock <= 0 ) {
		//std::cerr << "error\n"; // this is being dumb, it's some kind of error that I can't figure out
		perror("error");
    }
}

Webstream::~Webstream() {
    delete[] this;
}

void Webstream::bindSocket( const char *address, int port ) {
	sockaddr_in hint; // Hint is some kind of networking term, it's basically just the identifying info about a socket
	hint.sin_family = AF_INET;
	hint.sin_port = htons( port );

	inet_pton( AF_INET, address, &hint.sin_addr );
	bind( this->sock, (sockaddr *)&hint, sizeof( hint ) );
}

void Webstream::sendFrame(Mat frame) {
	//send(int __fd, const void *__buf, size_t __n, int __flags)
}
