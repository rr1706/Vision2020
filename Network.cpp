#ifdef WITH_NETWORK
#include "ntcore.h"
#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableInstance.h"
#include "Network.hpp"
#include "string"

std::shared_ptr<NetworkTable> myTable;

void startTable(){
	//figure out the deprication thing
	NetworkTable::SetClientMode();
	NetworkTable::SetIPAddress("10.17.6.2");
	NetworkTable::Initialize();
	myTable = NetworkTable::GetTable("SmartDashboard");
}

void sendDouble(std::string entryName, double input){
	myTable->PutNumber(entryName, input);
}
#endif