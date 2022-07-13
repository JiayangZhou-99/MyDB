#include "LockManager.hpp"

namespace MyDB
{
    bool LockManager::LockShared(Transaction *txn, const int& anBlockID){

        std::unique_lock<std::mutex> lock(LockManagerLatch);
        LockRequest theShareLockRequest(std::this_thread::get_id(),LockMode::SHARED,txn->getSequenceNumber());
        idToTransaction.emplace(std::this_thread::get_id(),txn);

        //when using IsolationLevel::READ_UNCOMMITTED no share lock will be needed.
        if (txn->GetIsolationLevel() == IsolationLevel::READ_UNCOMMITTED) {
            txn->SetState(TransactionState::ABORTED);
            throw TransactionAbortException(txn->GetThreadId(), AbortReason::LOCKSHARED_ON_UNCOMMITTED_READ);
            return false;
        }

        if (!LockPrepare(txn, anBlockID)) {
            return false;
        }

        LockRequestQueue *theRequestQueue = &lockTable.find(anBlockID)->second;
        theRequestQueue->request_queue_.emplace_back(theShareLockRequest);

        if (theRequestQueue->is_writing_) {

            // if there is a exclusive(write lock), block and wait uptil no one is writing or aborted
            DeadlockPrevent(txn, theRequestQueue);
            theRequestQueue->cv_.wait(lock, [theRequestQueue, txn]() -> bool {
                return txn->GetState() == TransactionState::ABORTED || !theRequestQueue->is_writing_;
            });

        }

        CheckAborted(txn, theRequestQueue);

        //add to the share lock set of the transaction to help you release
        txn->GetSharedLockSet()->emplace(anBlockID);
        theRequestQueue->sharing_count_++;

        auto iter = GetIterator(&theRequestQueue->request_queue_, txn->GetThreadId());
        iter->granted_ = true;

        return true;

    }


    bool LockManager::LockExclusive(Transaction *txn, const int& anBlockID){


        if(lockTable.count(anBlockID)!=0){
            LockRequestQueue *request_queue = &lockTable.find(anBlockID)->second;
            auto iter = GetIterator(&request_queue->request_queue_, txn->GetThreadId());
            if(iter != request_queue->request_queue_.end()){
                if(LockUpgrade(txn,anBlockID)){
                    return true;
                }
            }
        }

        std::unique_lock<std::mutex> lock(LockManagerLatch);
        LockRequest theExclusiveLock(std::this_thread::get_id(),LockMode::EXCLUSIVE,txn->getSequenceNumber());
        idToTransaction.emplace(std::this_thread::get_id(),txn);

        if (!LockPrepare(txn, anBlockID)) {
            return false;
        }

        LockRequestQueue *theRequestQueue = &lockTable.find(anBlockID)->second;
        theRequestQueue->request_queue_.emplace_back(theExclusiveLock);

        if (theRequestQueue->is_writing_ || theRequestQueue->sharing_count_ > 0) {
            // if there is any lock(exclusive or share), block and wait uptil no one is (writing and reading) or aborted
            DeadlockPrevent(txn, theRequestQueue);
            theRequestQueue->cv_.wait(lock, [theRequestQueue, txn]() -> bool {
                return txn->GetState() == TransactionState::ABORTED || (!theRequestQueue->is_writing_ && theRequestQueue->sharing_count_ == 0);
            });
        }

        CheckAborted(txn, theRequestQueue);

        //add to the share lock set of the transaction to help you release
        txn->GetExclusiveLockSet()->emplace(anBlockID);
        theRequestQueue->is_writing_ = true;

        auto iter = GetIterator(&theRequestQueue->request_queue_, txn->GetThreadId());
        iter->granted_ = true;

        return true;
    }
    
    bool LockManager::LockUpgrade(Transaction *txn, const int &anBlockID){
        std::unique_lock<std::mutex> lock(LockManagerLatch);

        if (txn->GetState() == TransactionState::SHRINKING) {
            txn->SetState(TransactionState::ABORTED);
            throw TransactionAbortException(txn->GetThreadId(), AbortReason::LOCK_ON_SHRINKING);
            return false;
        }

        LockRequestQueue *request_queue = &lockTable.find(anBlockID)->second;

        //if have been updated before
        if (request_queue->upgrading_) {
            txn->SetState(TransactionState::ABORTED);
            throw TransactionAbortException(txn->GetThreadId(), AbortReason::UPGRADE_CONFLICT);
            return false;
        }

        txn->GetSharedLockSet()->erase(anBlockID);
        request_queue->sharing_count_--;
        auto iter = GetIterator(&request_queue->request_queue_, txn->GetThreadId());
        iter->lock_mode_ = LockMode::EXCLUSIVE;
        iter->granted_ = false;

        if (request_queue->is_writing_ || request_queue->sharing_count_ > 0) {
            DeadlockPrevent(txn, request_queue);
            request_queue->upgrading_ = true;
            request_queue->cv_.wait(lock, [request_queue, txn]() -> bool {
                return txn->GetState() == TransactionState::ABORTED || (!request_queue->is_writing_ && request_queue->sharing_count_ == 0);
            });
        }

        CheckAborted(txn, request_queue);

        txn->GetExclusiveLockSet()->emplace(anBlockID);
        request_queue->upgrading_ = false;
        request_queue->is_writing_ = true;
        iter = GetIterator(&request_queue->request_queue_, txn->GetThreadId());
        iter->granted_ = true;

        return true;
    }

    void LockManager::CheckAborted(Transaction *txn, LockRequestQueue *request_queue){
        if (txn->GetState() == TransactionState::ABORTED) {
            auto iter = GetIterator(&request_queue->request_queue_, txn->GetThreadId());
            request_queue->request_queue_.erase(iter);
            throw TransactionAbortException(txn->GetThreadId(), AbortReason::DEADLOCK);
        }
    }

    bool LockManager::Unlock(Transaction *txn, const int &anBlockID){

        std::unique_lock<std::mutex> lock(LockManagerLatch);

        LockRequestQueue *request_queue = &lockTable.find(anBlockID)->second;

        txn->GetSharedLockSet()->erase(anBlockID);
        txn->GetExclusiveLockSet()->erase(anBlockID);

        auto iter = GetIterator(&request_queue->request_queue_, txn->GetThreadId());
        LockMode mode = iter->lock_mode_;
        request_queue->request_queue_.erase(iter);

        if (!(mode == LockMode::SHARED && txn->GetIsolationLevel() == IsolationLevel::READ_COMMITTED) &&
            txn->GetState() == TransactionState::GROWING) {
            txn->SetState(TransactionState::SHRINKING);
        }

        if (mode == LockMode::SHARED) {
            request_queue->sharing_count_ -= 1;
            if ( request_queue->sharing_count_ == 0) {
                request_queue->cv_.notify_all();
            }
        } else {
            request_queue->is_writing_ = false;
            request_queue->cv_.notify_all();
        }
        return true;
    }

    bool LockManager::LockPrepare(Transaction *txn, const int &anBlockID){

        if (txn->GetState() == TransactionState::SHRINKING) {
            txn->SetState(TransactionState::ABORTED);
            throw TransactionAbortException(txn->GetThreadId(), AbortReason::LOCK_ON_SHRINKING);
            return false;
        }

        if (lockTable.find(anBlockID) == lockTable.end()) {
            //if the block haven't been locked yet, create an entry point.
            lockTable.emplace(std::piecewise_construct, std::forward_as_tuple(anBlockID), std::forward_as_tuple());
        }
        return true;
    }

    std::list<LockRequest>::iterator LockManager::GetIterator(std::list<LockRequest> *request_queue, std::thread::id anThreadID){
        for (auto iter = request_queue->begin(); iter != request_queue->end(); ++iter) {
            if (iter->thread_id_ == anThreadID) {
                return iter;
            }
        }
        return request_queue->end();
    }

    void LockManager::DeadlockPrevent(Transaction *txn, LockRequestQueue *request_queue){
        for (const auto &request : request_queue->request_queue_) {
            if (request.granted_ && request.sequenceNumber > txn->getSequenceNumber()) {
                idToTransaction[request.thread_id_]->SetState(TransactionState::ABORTED);
                if (request.lock_mode_ == LockMode::SHARED) {
                    request_queue->sharing_count_--;
                } else {
                    request_queue->is_writing_ = false;
                }
            }
        }
    }

    void LockManager::registerTransaction(Transaction *txn){
        std::lock_guard<std::mutex> lock(LockManagerLatch);
        txn->setSequenceNumber(transactionSequenceNumber++);
    }

    void LockManager::lockLog(){
        std::lock_guard<std::mutex> lock(LockManagerLatch);
        rollOperationLatch.lock();
    }

    void LockManager::unlockLog(){
        std::lock_guard<std::mutex> lock(LockManagerLatch);
        rollOperationLatch.unlock();
    }

    // when dead lock detected abort all the transaction related to this database
    void  LockManager::abortAllTransaction(){
        std::lock_guard<std::mutex> lock(LockManagerLatch);
        for(auto& thePair:idToTransaction){
            thePair.second->SetState(TransactionState::ABORTED);
        }
        lockTable.clear();
    }

    void LockManager::deadLockDetection(Transaction *txn, LockRequestQueue *request_queue, LockMode theLockMode){
        // change the prevent lock to dfs dead lock detection
        if(theLockMode == LockMode::SHARED){
            for (const auto &request : request_queue->request_queue_) {
                if(request.lock_mode_ == LockMode::EXCLUSIVE && request.thread_id_ != txn->GetThreadId() && request.granted_){
                    dependencyGraph[txn->GetThreadId()].emplace(request.thread_id_);
                }
            }
        }else{
            for (const auto &request : request_queue->request_queue_) {
                if(request.thread_id_ != txn->GetThreadId() && request.granted_){
                    dependencyGraph[txn->GetThreadId()].emplace(request.thread_id_);
                }
            }
        }

        std::unordered_set<std::thread::id> theVisitedID;
        std::deque<std::thread::id> theThreadIDStack;
        theThreadIDStack.push_back(txn->GetThreadId());

        while(!theThreadIDStack.empty()){

            auto theCurNodeThreadID = theThreadIDStack.front();
            theThreadIDStack.pop_front();
            if(theVisitedID.count(theCurNodeThreadID)!=0){
                abortAllTransaction();
                return;
            }
            theVisitedID.emplace(theCurNodeThreadID);
            for(auto & theNextThreadID:dependencyGraph[theCurNodeThreadID]){
                theThreadIDStack.push_back(theNextThreadID);
            }
        }
        return;

    }

} // namespace MyDB
