#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <exception>
#include <string>
#include <thread>
#include <sstream>
#include <unordered_set>

namespace MyDB{
    /*
     * Transaction states for 2PL:
     *
     *     _________________________
     *    |                         v
     * GROWING -> SHRINKING -> COMMITTED   ABORTED
     *    |__________|________________________^
     *
     * Transaction states for Non-2PL:
     *     __________
     *    |          v
     * GROWING  -> COMMITTED     ABORTED
     *    |_________________________^
     *
     **/
    enum class TransactionState { GROWING, SHRINKING, COMMITTED, ABORTED };

    /*
     * Transaction isolation level.
     **/
    enum class IsolationLevel { READ_UNCOMMITTED, REPEATABLE_READ, READ_COMMITTED };

    /**
     * Type of write operation.
     */
    enum class WriteType { INSERT = 0, DELETE = 1, UPDATE = 2};

    /*
     Reason to a transaction abortion
     */
    enum class AbortReason {
        LOCK_ON_SHRINKING,
        UNLOCK_ON_SHRINKING,
        UPGRADE_CONFLICT,
        DEADLOCK,
        LOCKSHARED_ON_UNCOMMITTED_READ
    };

    //----------------------------------------------------------------------------//

    /*
     TransactionAbortException is thrown when state of a transaction is changed to ABORTED
    */
    class TransactionAbortException : public std::exception {

        std::thread::id     txn_id_;
        AbortReason         abort_reason_;

        public:
        explicit TransactionAbortException(std::thread::id txn_id, AbortReason abort_reason): txn_id_(txn_id), abort_reason_(abort_reason) {}

        std::thread::id GetTransactionThreadId() { return txn_id_; }

        AbortReason GetAbortReason() { return abort_reason_; }

        std::string GetInfo() {

            std::stringstream ss;
            ss<<txn_id_;
            switch (abort_reason_) {
                
            case AbortReason::LOCK_ON_SHRINKING:
                return "Transaction of thread "+ ss.str()+ "aborted because it can not take locks in the shrinking state\n";
            case AbortReason::UNLOCK_ON_SHRINKING:
                return "Transaction of thread "+ ss.str()+ "aborted because it can not excute unlock in the shrinking state\n";
            case AbortReason::UPGRADE_CONFLICT:
                return "Transaction of thread "+ ss.str()+ "aborted because another transaction is already waiting to upgrade its lock\n";
            case AbortReason::DEADLOCK:
                return "Transaction of thread "+ ss.str()+ "aborted on deadlock\n";
            case AbortReason::LOCKSHARED_ON_UNCOMMITTED_READ:
                return "Transaction of thread "+ ss.str()+ "aborted on lockshared on UNCOMMITTED_READ\n";
            }

            return "";
        }
    };

    //----------------------------------------------------------------------------//

    class Transaction //a transfer task containing serveral commands
    {
        public:
            Transaction(TransactionState ts,IsolationLevel isL, std::thread::id tID):transState(ts),isoLevel(isL),taskThreadID(tID){

                holdShareLocks      = std::make_shared<std::unordered_set<int>>();
                holdExclusiveLocks  = std::make_shared<std::unordered_set<int>>();
                
            }
            
            ~Transaction()=default;

            /** @return the id of the thread running the transaction */
            inline std::thread::id GetThreadId() const { return taskThreadID; }

            /** @return the isolation level of this transaction */
            inline IsolationLevel GetIsolationLevel() const { return isoLevel; }

            /** @return the set of resources under a shared lock */
            inline std::shared_ptr<std::unordered_set<int>> GetSharedLockSet() { return holdShareLocks; }

            /** @return the set of resources under an exclusive lock */
            inline std::shared_ptr<std::unordered_set<int>> GetExclusiveLockSet() { return holdExclusiveLocks; }

            /** @return true if rid is shared locked by this transaction */
            bool IsSharedLocked(const int& anBlockIndex) { return holdShareLocks->find(anBlockIndex) != holdShareLocks->end(); }

            /** @return true if rid is exclusively locked by this transaction */
            bool IsExclusiveLocked(const int& anBlockIndex) { return holdExclusiveLocks->find(anBlockIndex) != holdExclusiveLocks->end(); }

            /** @return the current state of the transaction */
            inline TransactionState GetState() { return transState; }

            /**
             * Set the state of the transaction.
             * @param state new state
             */
            inline void SetState(TransactionState state) { transState = state; }

            int getSequenceNumber(){return transactionSequenceNumber;}

            void setSequenceNumber(int anSecNumber){transactionSequenceNumber = anSecNumber;}

            void clear(){holdExclusiveLocks->clear();holdExclusiveLocks->clear();}

        private:
            /* data */
            int                 transactionSequenceNumber;
            TransactionState    transState;
            IsolationLevel      isoLevel;
            std::thread::id     taskThreadID;

            /* LockManager: the set of shared-locked cache block held by this transaction. */
            std::shared_ptr<std::unordered_set<int>> holdShareLocks = nullptr;
            /* LockManager: the set of exclusive-locked cache block held by this transaction. */
            std::shared_ptr<std::unordered_set<int>> holdExclusiveLocks = nullptr;
    };

    using TransactionPtr= std::shared_ptr<Transaction>;
    
}
#endif