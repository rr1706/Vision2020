// basically save a file then push file to network
#pragma once

class Webstream {
	public:
	Webstream();
	~Webstream();

	void bindSocket( const char *address, int port );


	// memset( buf, 0, 4096 );

	private:
	int server;
	char buf[4096];
};