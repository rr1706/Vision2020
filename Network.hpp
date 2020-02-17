#ifdef NETWORK
#include "ntcore.h"
#include "networktables/NetworkTable.h"
#include "string"

using namespace std;
using std::shared_ptr;

void sendDouble(string entry, double input);
void sendString(string entryName, string input);
string pullString(string entryName, string defaultValue);
double pullDouble(string entryName, double defaultValue);
void startTable();
#endif