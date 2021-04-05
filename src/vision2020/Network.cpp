#ifdef WITH_NETWORK
#include "include/Network.hpp"

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
#endif // WITH_NETWORK
