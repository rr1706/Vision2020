#ifdef WITH_NETWORK
#include "headers/Network.hpp"
#include "lua.h"

/*
void Network::Network() {
	// figure out the deprication thing
	NetworkTable::SetClientMode();
	NetworkTable::SetIPAddress( "10.17.6.2" ); // make sure this is right
	NetworkTable::Initialize();
	Network::myTable = NetworkTable::GetTable( "SmartDashboard" );
}

void Network::sendDouble( std::string entryName, double input ) {
	Network::myTable->PutNumber( entryName, input );
	Network::myTable->flush();
}

void Network::sendString( std::string entryName, std::string input ) {
	Network::myTable->PutString( entryName, input );
	Network::myTable->flush();
}
*/

void Network::Network() {
    
    lua_State *l;
    l = lua_open();
    lua_dofile(l, "sockets.lua");    
    lua_close(l);
    
}

void Network::sendDouble( std::string entryName, double input ) {
	
}

void Network::sendString( std::string entryName, std::string input ) {
	
}

#endif // WITH_NETWORK
