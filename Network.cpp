#include "ntcore.h"
#include "networktables/NetworkTable.hpp"
#include "Network.hpp"

using std::shared_ptr;

NetworkTable::SetClientMode();
//ip = 10.te.am.2
NetworkTable::SetIpAddress("10.17.6.2");
NetworkTable::Initalize();

//this will add to the smart dashboard which can be pulled from the the driver station
void sendDouble(string entry, double input){
    myTable->NetworkTable::PutString( entry, to_string(input) );
}