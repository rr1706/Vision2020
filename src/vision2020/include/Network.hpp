#pragma once

#include <iostream>
#include <string>

#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableInstance.h"
#include "ntcore.h"

class Network{
  public:
    void sendDouble(std::string entry, double input);
    void sendString(std::string entryName, std::string input);
    void init();
  
  private:
    std::shared_ptr<NetworkTable> myTable;
};