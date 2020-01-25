#include "ntcore.h"
#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableInstance.h"
#include "Network.hpp"
#include "string"

using std::shared_ptr;
using namespace std;

NetworkTable::SetClientMode();
//ip = 10.te.am.2
NetworkTable::SetIpAddress("10.17.6.2");
NetworkTable::Initalize();

//this will add to the smart dashboard which can be pulled from the the driver station
void sendMessage(string entry, double input){
	shared_ptr<NetworkTable> myTable = NetworkTable::GetTable("SmartDashboard");
    	myTable->NetworkTable::PutString( entry, to_string(input) );
}
