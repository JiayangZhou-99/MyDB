//
//  main.cpp
//  
//


#include <iostream>
#include <string>
#include <stdio.h>
#include "SQLServer.hpp"


int main(int argc, const char * argv[]) {

  srand(static_cast<uint32_t>(time(0)));
    
  if (argc != 3) {                
    std::cout << "Usage: " << argv[0] << " <Server Address> <Server Port> " << std::endl;
    exit(1);
  }

  std::string     theServerAddr = argv[1];             // First arg:  Address
  unsigned short  theServerPort = atoi(argv[2]);       // Second arg: Port
  MyDB::SQLServer theSQLServer(theServerAddr,theServerPort);
  theSQLServer.run();
  
}
