#include "ntcore.h"
#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableInstance.h"
#include "Network.hpp"
#include "string"
//#include <StringRef.h>

using std::shared_ptr;
using namespace std;
//using namespace wpi;

shared_ptr<NetworkTable> myTable;

void startTable(){
	NetworkTable::SetClientMode();
	NetworkTable::SetIPAddress("10.17.6.2");
	NetworkTable::Initialize();
	myTable = NetworkTable::GetTable("SmartDashboard");
}

void sendMessage(string entryName, double input){
	myTable->PutNumber(entryName, input);
}
