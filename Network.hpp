#include "ntcore.h"
#include "networktables/NetworkTable.hpp"

using std::shared_ptr;

shared_ptr<NetworkTable> myTable = NetworkTable::getTable("Network");
void sendDouble(string entry, double input);