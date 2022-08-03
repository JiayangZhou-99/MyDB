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
    
  if (argc != 4) {                
    std::cout << "Usage: " << argv[0] << " <Server Address> <Server Port> <Connection Mechenism> " << std::endl;
    exit(1);
  }

  std::string     theServerAddr = argv[1];             // First arg:  Address
  unsigned short  theServerPort = atoi(argv[2]);       // Second arg: Port
  std::string     theConnTerm  = argv[3];
  std::transform(theConnTerm.begin(),theConnTerm.end(),theConnTerm.begin(),::tolower);
  MyDB::ConnTerm  theConnectionMechenism;
  if(theConnTerm == "long"){
    theConnectionMechenism = MyDB::ConnTerm::LongConn;
  }else{
    theConnectionMechenism = MyDB::ConnTerm::shortConn;
  }

  MyDB::SQLServer theSQLServer(theServerAddr,theServerPort,theConnectionMechenism);
  theSQLServer.run();
  
}
