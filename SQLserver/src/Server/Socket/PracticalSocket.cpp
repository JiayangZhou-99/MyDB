//
//  PracticalSocket.cpp
//

#include "PracticalSocket.hpp"

/*
 *   C++ sockets
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

#ifdef WIN32
  #include <winsock.h>         // For socket(), connect(), send(), and recv()
  typedef int socklen_t;
  typedef char raw_type;       // Type used for raw data on this platform
#else
  #include <sys/types.h>       // For data types
  #include <sys/socket.h>      // For socket(), connect(), send(), and recv()
  #include <netdb.h>           // For gethostbyname()
  #include <arpa/inet.h>       // For inet_addr()
  #include <unistd.h>          // For close()
  #include <netinet/in.h>      // For sockaddr_in
  typedef void raw_type;       // Type used for raw data on this platform
#endif

#include <errno.h>             // For errno


#ifdef WIN32
static bool initialized = false;
#endif

// SocketException Code

SocketException::SocketException(const std::string &message, bool inclSysMsg): userMessage(message) {
  if (inclSysMsg) {
    userMessage.append(": ");
    userMessage.append(strerror(errno));
  }
}

SocketException::~SocketException(){
}

const char *SocketException::whatError() const{
  return userMessage.c_str();
}

// Function to fill in address structure given an address and port
static void fillAddr(const std::string &address, unsigned short port,
                     sockaddr_in &addr) {
  memset(&addr, 0, sizeof(addr));  // Zero out address structure
  addr.sin_family = AF_INET;       // Internet address

  hostent *host;  // Resolve name
  if ((host = gethostbyname(address.c_str())) == NULL) {
    // strerror() will not work for gethostbyname() and hstrerror()
    // is supposedly obsolete
    throw SocketException("Failed to resolve name (gethostbyname())");
  }
  addr.sin_addr.s_addr = *((unsigned long *) host->h_addr_list[0]);

  addr.sin_port = htons(port);     // Assign port in network byte order
}

// Socket Code

Socket::Socket(int type, int protocol){
  #ifdef WIN32
    if (!initialized) {
      WORD wVersionRequested;
      WSADATA wsaData;

      wVersionRequested = MAKEWORD(2, 0);              // Request WinSock v2.0
      if (WSAStartup(wVersionRequested, &wsaData) != 0) {  // Load WinSock DLL
        throw SocketException("Unable to load WinSock DLL");
      }
      initialized = true;
    }
  #endif

  // Make a new socket
  if ((sockDesc = socket(PF_INET, type, protocol)) < 0) {
    throw SocketException("Socket creation failed (socket())", true);
  }
}

Socket::Socket(int sockDesc) {
  this->sockDesc = sockDesc;
}

Socket::~Socket() {
  #ifdef WIN32
    ::closesocket(sockDesc);
  #else
    ::close(sockDesc);
  #endif
  sockDesc = -1;
}

std::string Socket::getLocalAddress(){
  sockaddr_in addr;
  unsigned int addr_len = sizeof(addr);

  if (getsockname(sockDesc, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) {
    throw SocketException("Fetch of local address failed (getsockname())", true);
  }
  return inet_ntoa(addr.sin_addr);
}

unsigned short Socket::getLocalPort(){
  sockaddr_in addr;
  unsigned int addr_len = sizeof(addr);

  if (getsockname(sockDesc, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) {
    throw SocketException("Fetch of local port failed (getsockname())", true);
  }
  return ntohs(addr.sin_port);
}

void Socket::setLocalPort(unsigned short localPort){
  // Bind the socket to its port
  sockaddr_in localAddr;
  memset(&localAddr, 0, sizeof(localAddr));
  localAddr.sin_family = AF_INET;
  localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  localAddr.sin_port = htons(localPort);

  if (::bind(sockDesc, (sockaddr *) &localAddr, sizeof(sockaddr_in)) == -1){
    throw SocketException("Set of local port failed (bind())", true);
  }
}

void Socket::setLocalAddressAndPort(const std::string &localAddress,
    unsigned short localPort){
  // Get the address of the requested host
  sockaddr_in localAddr;
  fillAddr(localAddress, localPort, localAddr);

  if (::bind(sockDesc, (sockaddr *) &localAddr, sizeof(sockaddr_in)) < 0) {
    throw SocketException("Set of local address and port failed (bind())", true);
  }
}

void Socket::cleanUp(){
  #ifdef WIN32
    if (WSACleanup() != 0) {
      throw SocketException("WSACleanup() failed");
    }
  #endif
}

unsigned short Socket::resolveService(const std::string &service,
                                      const std::string &protocol) {
  struct servent *serv;        /* Structure containing service information */

  if ((serv = getservbyname(service.c_str(), protocol.c_str())) == NULL)
    return atoi(service.c_str());  /* Service is port number */
  else
    return ntohs(serv->s_port);    /* Found port (network byte order) by name */
}

// CommunicatingSocket Code

CommunicatingSocket::CommunicatingSocket(int type, int protocol)
    : Socket(type, protocol) {
}

CommunicatingSocket::CommunicatingSocket(int newConnSD) : Socket(newConnSD) {
}

void CommunicatingSocket::connect(const std::string &foreignAddress,
    unsigned short foreignPort) {
  // Get the address of the requested host
  sockaddr_in destAddr;
  fillAddr(foreignAddress, foreignPort, destAddr);

  // Try to connect to the given port
  if (::connect(sockDesc, (sockaddr *) &destAddr, sizeof(destAddr)) < 0) {
    throw SocketException("Connect failed (connect())", true);
  }
}

void CommunicatingSocket::send(const void *buffer, int bufferLen){
  if (::send(sockDesc, (raw_type *) buffer, bufferLen, 0) < 0) {
    throw SocketException("Send failed (send())", true);
  }
}

int CommunicatingSocket::recv(void *buffer, int bufferLen){
  int rtn;
  if ((rtn = ::recv(sockDesc, (raw_type *) buffer, bufferLen, 0)) < 0) {
    throw SocketException("Received failed (recv())", true);
  }

  return rtn;
}

std::string CommunicatingSocket::getForeignAddress(){
  sockaddr_in addr;
  unsigned int addr_len = sizeof(addr);

  if (getpeername(sockDesc, (sockaddr *) &addr,(socklen_t *) &addr_len) < 0) {
    throw SocketException("Fetch of foreign address failed (getpeername())", true);
  }
  return inet_ntoa(addr.sin_addr);
}

unsigned short CommunicatingSocket::getForeignPort() {
  sockaddr_in addr;
  unsigned int addr_len = sizeof(addr);

  if (getpeername(sockDesc, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) {
    throw SocketException("Fetch of foreign port failed (getpeername())", true);
  }
  return ntohs(addr.sin_port);
}

// TCPSocket Code

TCPSocket::TCPSocket(): CommunicatingSocket(SOCK_STREAM,
    IPPROTO_TCP) {
}

TCPSocket::TCPSocket(const std::string &foreignAddress, unsigned short foreignPort): CommunicatingSocket(SOCK_STREAM, IPPROTO_TCP) {
  connect(foreignAddress, foreignPort);
}

TCPSocket::TCPSocket(int newConnSD) : CommunicatingSocket(newConnSD) {
}

// TCPServerSocket Code

TCPServerSocket::TCPServerSocket(unsigned short localPort, int queueLen): Socket(SOCK_STREAM, IPPROTO_TCP) {
  setLocalPort(localPort);
  setListen(queueLen);
}

TCPServerSocket::TCPServerSocket(const std::string &localAddress,
    unsigned short localPort, int queueLen): Socket(SOCK_STREAM, IPPROTO_TCP) {
  setLocalAddressAndPort(localAddress, localPort);
  setListen(queueLen);
}

TCPSocket *TCPServerSocket::accept(){
  int newConnSD;
  if ((newConnSD = ::accept(sockDesc, NULL, 0)) < 0) {
    throw SocketException("Accept failed (accept())", true);
  }

  return new TCPSocket(newConnSD);
}

void TCPServerSocket::setListen(int queueLen){
  if (listen(sockDesc, queueLen) < 0) {
    throw SocketException("Set listening socket failed (listen())", true);
  }
}

