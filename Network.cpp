#include "ntcore.h"
#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableInstance.h"
#include "Network.hpp"
#include "string"

using std::shared_ptr;
using namespace std;

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

void sendString(string entryName, string input){
	myTable->PutString(entryName, input);
}