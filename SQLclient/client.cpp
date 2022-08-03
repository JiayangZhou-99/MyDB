//
//  main.cpp
//  SimpleSerevr
//
//  Created by 周家阳 on 6/30/22.
//

/*
 *   C++ sockets on Unix and Windows
 *   Copyright (C) 2002
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "PracticalSocket.hpp"  // For Socket and SocketException
#include <iostream>           // For cerr and cout
#include <cstdlib>            // For atoi()

const int RCVBUFSIZE = 4096;    // Size of receive buffer
enum class ConnTerm{shortConn = 0,LongConn = 1};

int main(int argc, char *argv[]) {

  if ( argc != 4) {     // Test for correct number of arguments
    std::cout << "Usage: " << argv[0] << " <Server> <Server Port> <Connection Mechenism>" << std::endl;
    exit(1);
  }

  std::string  servAddress = argv[1]; // First arg: server address
  std::string  commandString;   // Second arg: string to echo
  unsigned short serverPort = std::atoi(argv[2]);
  std::string     theConnTerm  = argv[3];
  std::transform(theConnTerm.begin(),theConnTerm.end(),theConnTerm.begin(),::tolower);
  ConnTerm  theConnectionMechenism;

  if(theConnTerm == "long"){
    theConnectionMechenism = ConnTerm::LongConn;
  }else{
    theConnectionMechenism = ConnTerm::shortConn;
  }

  std::unique_ptr<TCPSocket> sockPtr;
  try {
    // Establish connection with the echo server
    if(theConnectionMechenism == ConnTerm::LongConn){
      sockPtr = std::make_unique<TCPSocket>(servAddress, serverPort);
      std::cout << "long connected to server" << std::endl;
    }

    while(true){
      // Send the string to the echo server
      std::getline(std::cin, commandString);
      
      if(theConnectionMechenism == ConnTerm::shortConn){
        sockPtr = std::make_unique<TCPSocket>(servAddress, serverPort);
        std::cout << "short connected to server" << std::endl;
      }

      sockPtr->send(const_cast<char*>(commandString.c_str()), commandString.size());
  
      char receiveBuffer[RCVBUFSIZE + 1];    // Buffer for echo string + \0
      int  bytesReceived = 0;              // Bytes read on each recv()
      int  totalBytesReceived = 0;         // Total bytes read
      // Receive the same string back from the server
      std::cout << "Received: \n";               // Setup to print the echoed string
      bytesReceived = sockPtr->recv(receiveBuffer, RCVBUFSIZE);

      while (bytesReceived == RCVBUFSIZE) {    
        totalBytesReceived += bytesReceived;     // Keep tally of total bytes
        receiveBuffer[bytesReceived] = '\0';        // Terminate the string!
        std::cout << receiveBuffer;                 // Print the echo buffer
        memset(receiveBuffer,'\0',sizeof(receiveBuffer));
        bytesReceived = sockPtr->recv(receiveBuffer, RCVBUFSIZE);
      }

      if (bytesReceived  < 0) {
        std::cout << "Unable to read";
        exit(1);
      } 

      totalBytesReceived += bytesReceived;     // Keep tally of total bytes
      receiveBuffer[bytesReceived] = '\0';        // Terminate the string!
      std::cout << receiveBuffer;                 // Print the echo buffer


      std::cout << std::endl;
      memset(receiveBuffer,'\0',sizeof(receiveBuffer));
      if(theConnectionMechenism == ConnTerm::shortConn) sockPtr = nullptr;
      if(std::string(commandString) == "quit;"){
        return 0;
      }
    }
    // Destructor closes the socket
  } catch(SocketException &e) {
    std::cerr << e.whatError() << std::endl;
    exit(1);
  }

  return 0;
}
