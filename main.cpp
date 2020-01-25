#include "opencv2/opencv.hpp"
#include <iostream>
#include "networktables/NetworkTable.h"
#include "ntcore.h"

using std::shared_ptr;
using namespace std;


char esc;
int main() {
	double num = 0.0;
    NetworkTable::SetClientMode();
   	NetworkTable::SetIPAddress("10.17.6.2");
    NetworkTable::Initialize();
    	
	while(true){
		shared_ptr<NetworkTable> myTable = NetworkTable::GetTable("SmartDashboard");
    		myTable->NetworkTable::PutString("Double", to_string(num));
		string val = myTable->NetworkTable::GetString("Double", "0");
		cout << num << endl;
		cout << val << endl;
		esc = cv::waitKey(27);
		if(esc == 33){
			break;
		}
		num = num + 1.1;
    	}
}
