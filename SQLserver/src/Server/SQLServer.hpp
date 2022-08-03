//Server.hpp

#ifndef SQLServer_hpp
#define SQLServer_hpp

// #include "TestManually.hpp"
#include "TestAutomatic.hpp"
#include "PracticalSocket.hpp"
#include <unordered_map>
#include <thread>
#include "ThreadPool.hpp"

// output colorful "FAIL" and "PASS"
#define WHITE "\033[37m"
#define RED "\033[31m"
#define GREEN "\033[32m"

namespace MyDB{
    
    const int RCVBUFSIZE = 4096;
    const size_t numOfThreads = 8;
    enum class ConnTerm{shortConn = 0,LongConn = 1};
    using ThreadPoolPtr = std::unique_ptr<ThreadPool>;

    class SQLServer{
        using applicationPtr = std::shared_ptr<MyDB::Application>;
    public:
        SQLServer(const std::string &foreignAddress, unsigned short foreignPort,ConnTerm anTerm);
        void run();

        /**
         * client connection handler, start an new application to receive and run the command
        */
        ///////////// have a bug here !!!!!!!!!!!!!!!!!!!!!!!!!!
        ///////////// have a bug here !!!!!!!!!!!!!!!!!!!!!!!!!!
        ///////////// have a bug here !!!!!!!!!!!!!!!!!!!!!!!!!!
        ///////////// have a bug here !!!!!!!!!!!!!!!!!!!!!!!!!!
        ///////////// have a bug here !!!!!!!!!!!!!!!!!!!!!!!!!!
        ///////////// have a bug here !!!!!!!!!!!!!!!!!!!!!!!!!!
        ///////////// have a bug here !!!!!!!!!!!!!!!!!!!!!!!!!!
        // all client is reusing the same handle function!

        static void handleTCPConnection(TCPSocket *sock){

            std::cout << "Handling client ";
            try {
                std::cout << sock->getForeignAddress() << ":";
            } catch (SocketException &e) {
                std::cerr << "Unable to get foreign address" << std::endl;
            }

            try {
                std::cout << sock->getForeignPort();
            } catch (SocketException &e) {
                std::cerr << "Unable to get foreign port" << std::endl;
            }
            std::cout << " with thread " << pthread_self() << std::endl;

            // Send received string and receive again until the end of transmission
            char                receiveBuffer[RCVBUFSIZE];
            int                 recvMsgSize;
            std::stringstream   theOutput;
            MyDB::Application   theApp(theOutput);
            // applicationPtr      theAppPtr = std::make_shared<MyDB::Application>(theOutput);
            MyDB::StatusResult  theResult{};
            bool                running=true;

            do {

                MyDB::TestAutomatic theTests(theOutput);

                while ((recvMsgSize = sock->recv(receiveBuffer, RCVBUFSIZE)) > 0) {
                    // std::cout << "------------------ "<< std::endl;
                    // std::cout << theAppPtr->getOutput()<< std::endl;
                    // std::cout << "------------------ "<< std::endl;

                    using TestCall = std::function<bool()>;
                    static std::map<std::string, TestCall> theCalls {
                        {"App Test",         [&](){return theTests.doAppTest();}  },
                        {"AddColumn Test",   [&](){return theTests.doAddColumnTest();}  },
                        {"BlockCache Test",  [&](){return theTests.doBlockCacheTest();}  },
                        {"Compile Test",     [&](){return theTests.doCompileTest();}  },
                        {"Delete Test",      [&](){return theTests.doDeleteTest();}  },
                        {"DropTable Test",   [&](){return theTests.doDropTest();}  },
                        {"DBCommands Test",  [&](){return theTests.doDBCommandsTest();}  },
                        {"Index Test",       [&](){return theTests.doIndexTest();}},
                        {"Insert Test",      [&](){return theTests.doInsertTest();}  },
                        {"Joins Test",       [&](){return theTests.doJoinTest();}  },
                        {"Reader Test",      [&](){return theTests.doReaderTest();}  },
                        {"RemoveColumn Test",[&](){return theTests.doRemoveColumnTest();}  },
                        {"RowCache Test",    [&](){return theTests.doRowCacheTest();}  },
                        {"Select Test",      [&](){return theTests.doSelectTest();}  },
                        {"Tables Test",      [&](){return theTests.doTablesTest();}  },
                        {"Update Test",      [&](){return theTests.doUpdateTest();}  },
                        {"ViewCache Test",   [&](){return theTests.doViewCacheTest();}  },
                    };

                    std::string theCmd(std::string(receiveBuffer).substr(0,recvMsgSize));

                    if(theCalls.count(theCmd)) {
                        bool theResult = theCalls[theCmd]();
                        const char* theStatus[]={"FAIL","PASS"};
                        theOutput << theCmd << " test " << theStatus[theResult] << "\n";
                        theOutput << "------------------------------\n";
                    }else if(theCmd == "Run All Test"){
                        for(auto& theTestPair:theCalls){
                            bool theResult = theTestPair.second();
                            const char* theStatus[]={"FAIL","PASS"};
                            const char* theColor[] = {RED, GREEN};
                            std::cout << theTestPair.first << " \t\t" << theColor[theResult] <<theStatus[theResult]<< WHITE << "\n";
                        }
                        running = false;
                    }else{
                        std::cout << "Command Received: "<< theCmd << "\n> ";
                        if(theCmd.length()) {
                            std::stringstream theStream(theCmd);
                            theResult=theApp.handleInput(theStream);
                            if(theResult==MyDB::userTerminated) {
                                running=false;
                            }
                            else if(!theResult) {
                                MyDB::showErrors(theResult, theOutput);
                            }
                        }
                        memset(receiveBuffer,0,sizeof(receiveBuffer));
                    }

                    std::string theReplyMsg = theApp.getOutput();
                    // std::cout << theReplyMsg << std::endl;
                    // portLatch.lock();
                    sock->send(const_cast<char*>(theReplyMsg.c_str()), theReplyMsg.size());
                    // portLatch.unlock();
                    theApp.refresh();
                    if(running == false) break;
                }
                // Destructor closes socketbb
            }while (running);
            delete sock;
        };


        static void makeThread(TCPSocket* clntSock,ConnTerm anTerm){

            // Guarantees that thread resources are deallocated upon return
            // pthread_detach(pthread_self());

            // Extract socket file descriptor from argument
            if(anTerm == ConnTerm::shortConn){
                serverThreadPoolPtr->submit(SQLServer::handleTCPConnection,clntSock);
            }else{
                handleTCPConnection(clntSock);
            }
            // delete clntSock;
        }
        
    private:

        static ThreadPoolPtr serverThreadPoolPtr;
        static std::mutex portLatch;
        std::unique_ptr<TCPServerSocket> SQLServerSocketPtr;
        ConnTerm term;

    };
}

#endif