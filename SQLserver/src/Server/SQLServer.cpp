// Server.cpp

#include "SQLServer.hpp"
#include "DatabaseInstanceManager.hpp"

namespace MyDB {

    std::mutex      SQLServer::portLatch;
    ThreadPoolPtr   SQLServer::serverThreadPoolPtr = std::make_unique<ThreadPool>(numOfThreads);

    SQLServer::SQLServer(const std::string &foreignAddress, unsigned short foreignPort,ConnTerm anTerm){
        SQLServerSocketPtr = std::make_unique<TCPServerSocket>(foreignAddress,foreignPort);
        term = anTerm;
    }

    void SQLServer::run(){
        if(term == ConnTerm::LongConn) serverThreadPoolPtr = nullptr;
        try{
            for (;;) {      // Run
                std::cout << "waiting for new client"<< std::endl;
                TCPSocket* clntSocket = SQLServerSocketPtr->accept();
                // Create client thread
                if(term == ConnTerm::LongConn){
                    std::thread t1(makeThread, clntSocket,term);
                    t1.detach();
                }else{
                     makeThread(clntSocket,term);
                }

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
