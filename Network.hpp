#ifdef WITH_NETWORK
#include "ntcore.h"
#include "networktables/NetworkTable.h"
//#include "string"

//using namespace std;
using std::shared_ptr;

void sendDouble(string entry, double input);
void sendString(string entryName, string input);
void startTable();
#endif