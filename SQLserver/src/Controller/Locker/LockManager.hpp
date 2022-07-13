//global static 
#ifndef LOCKMANAGER_HPP
#define LOCKMANAGER_HPP

#include <list>
#include <mutex>
#include "Transaction.hpp"
#include <unordered_map>
#include <unordered_set>
#include <deque>


namespace MyDB
{

    enum class LockMode { SHARED, EXCLUSIVE };

    //lock request from a transaction
    class LockRequest {
    public:
        LockRequest(std::thread::id anThreadID, LockMode lock_mode, int anSeqNum) : sequenceNumber(anSeqNum), thread_id_(anThreadID), lock_mode_(lock_mode) {}

        int                sequenceNumber;
        std::thread::id    thread_id_;
        LockMode           lock_mode_;
        bool               granted_=false;
    };


    //LockRequest for the same resource
    class LockRequestQueue {
    public:
        std::list<LockRequest> request_queue_;
        // for notifying blocked transactions on this rid
        std::condition_variable cv_;
        bool upgrading_ = false;
        int  sharing_count_ = 0;
        bool is_writing_ = false;
    };



    class LockManager{
    public:
        /**
         * Creates a new lock manager configured for the deadlock prevention policy.
         */
        LockManager() = default;

        ~LockManager() = default;

        /**
         * @param txn the transaction requesting the shared lock
         * @param anBlockID the ID to be locked in shared mode
         * @return true if the lock is granted, false otherwise
         */
        bool LockShared(Transaction *txn, const int& anBlockID);

        /**
         * @param txn the transaction requesting the exclusive lock
         * @param anBlockID the ID to be locked in exclusive mode
         * @return true if the lock is granted, false otherwise
         */
        bool LockExclusive(Transaction *txn, const int& anBlockID);

        /**
         * @param txn the transaction requesting the lock updrade
         * @param anBlockID the ID to be ungraded
         * requesting transaction
         * @return true if the upgrade is successful, false otherwise
         */
        bool LockUpgrade(Transaction *txn, const int &anBlockID);

        /**
         * Check if the status of the transaction is set to Aborted, and throw an exception if it is.
         */
        void CheckAborted(Transaction *txn, LockRequestQueue *request_queue);

        /**
         * @param txn the transaction requesting unlock
         * @param anBlockID the ID is locked.
         * @return true if the unlock is successful, false otherwise.
         */
        bool Unlock(Transaction *txn, const int &anBlockID);

        void registerTransaction(Transaction *txn);

        /**
         * used for synchronizing all the redo and undo operation for a single database;
         */
        void lockLog();

        void unlockLog();

        void abortAllTransaction();

        
        /**
         * used for detecting deadlock withing the lock manager, if happened abort all the transactions.
         */
        void deadLockDetection(Transaction *txn,LockRequestQueue *request_queue,LockMode theLockMode);

    private:

        bool LockPrepare(Transaction *txn, const int &anBlockID);

        std::list<LockRequest>::iterator GetIterator(std::list<LockRequest> *request_queue, std::thread::id anThreadID);

        void DeadlockPrevent(Transaction *txn, LockRequestQueue *request_queue);

        std::mutex LockManagerLatch;

        std::mutex rollOperationLatch;
        
        int        transactionSequenceNumber = 0;

        /** Lock table for lock requests. 
         * from block number to lockRequestQueue
        */
        std::unordered_map<int, LockRequestQueue> lockTable;

        //threadID to transaction
        std::unordered_map<std::thread::id, Transaction*> idToTransaction;

        std::unordered_map<std::thread::id,std::unordered_set<std::thread::id>> dependencyGraph;
    };
    
} // namespace MyDB

#endif
