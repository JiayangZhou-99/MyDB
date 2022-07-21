// Server.cpp

#include "SQLServer.hpp"
#include "DatabaseInstanceManager.hpp"

namespace MyDB {

    std::mutex SQLServer::portLatch;
    ThreadPool SQLServer::serverThreadPool(numOfThreads);

    SQLServer::SQLServer(const std::string &foreignAddress, unsigned short foreignPort){
        SQLServerSocketPtr = std::make_unique<TCPServerSocket>(foreignAddress,foreignPort);
    }

    void SQLServer::run(){
        try{
            for (;;) {      // Run
                std::cout << "waiting for new client"<< std::endl;
                TCPSocket* clntSocket = SQLServerSocketPtr->accept();
                // Create client thread
                std::thread t1(makeThread, clntSocket);
                t1.detach();

                // t1.join();
                // std::cout << "new thread created"<< std::endl;

                // pthread_t threadID;       
                // if (pthread_create(&threadID, NULL, ThreadMain,(void *) clntSocket) != 0) {
                //   std::cerr << "Unable to create thread" << std::endl;
                //   exit(1);
                // }
            }
        }catch (SocketException &e) {
            std::cout << "error happened: " << std::endl;
            std::cout << e.whatError() << std::endl;
            exit(1);
        }
    }
}
