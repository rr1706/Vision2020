// I got a lot of help from this tutorial https://www.youtube.com/watch?v=cNdlrbZSkyQ&t=1142s
#include "headers/WebStream.hpp"

#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

Webstream::Webstream() {
	int server = socket( AF_INET, SOCK_STREAM, 0 );
	if( server < 0 ) { std::cerr << "can't create socket" << std::endl; }
}

Webstream::~Webstream() {}

void Webstream::bindSocket( const char *address, int port ) {
	sockaddr_in hint; // Hint is some kind of networking term, it's basically just the identifying info about a socket
	hint.sin_family = AF_INET;
	hint.sin_port = htons( port );

	inet_pton( AF_INET, address, &hint.sin_addr );
	bind( server, (sockaddr *)&hint, sizeof( hint ) );

	if( listen( server, SOMAXCONN ) < 0 ) { std::cerr << "can't listen\n"; }
}