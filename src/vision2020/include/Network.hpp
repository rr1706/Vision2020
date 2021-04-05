#pragma once

#include <iostream>
#include <string>

#include "Poco/Net/SocketAddress.h"

class Network{
  public:
    Network();
    void sendDouble(std::string entry, double input);
    void sendString(std::string entryName, std::string input);
  
  private:
    std::shared_ptr<NetworkTable> myTable;
};
