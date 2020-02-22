#ifdef WITH_NETWORK
#include "ntcore.h"
#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableInstance.h"
#include "Network.hpp"
#include "string"

using std::shared_ptr;
using namespace std;

shared_ptr<NetworkTable> myTable;

void startTable(){
	//figure out the deprication thing
	NetworkTable::SetClientMode();
	NetworkTable::SetIPAddress("10.17.6.2");
	NetworkTable::Initialize();
	myTable = NetworkTable::GetTable("SmartDashboard");
}

void sendDouble(string entryName, double input){
	myTable->PutNumber(entryName, input);
}

void sendString(string entryName, string input){
	myTable->PutString(entryName, input);
}
#endif