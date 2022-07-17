#ifndef DATABASEINSTANCEMANAGER_HPP
#define DATABASEINSTANCEMANAGER_HPP

#include <memory>
#include <unistd.h>
#include <thread>
#include <unordered_map>
#include <mutex>
#include "Database.hpp"
#include "Config.hpp"
#include "FolderReader.hpp"

// remove the singleton pattern


namespace MyDB {

	using threadID = std::thread::id;

	class DatabaseInstanceManager {
	protected:
		static inline std::unordered_map<threadID,DatabasePtr> threadIDToDBPtrMap;
		// static inline DatabasePtr activeDatabase;
		static std::mutex databaseInstanceManagerLock;

	public:

		static inline DatabaseRawPtr getActiveDatabase() {
			std::lock_guard<std::mutex> theLG(DatabaseInstanceManager::databaseInstanceManagerLock);
			threadID theThreadID = std::this_thread::get_id();
			return DatabaseInstanceManager::threadIDToDBPtrMap[theThreadID].get();
		}

		static inline bool hasActiveDatabase() {
			std::lock_guard<std::mutex> theLG(DatabaseInstanceManager::databaseInstanceManagerLock);
			threadID theThreadID = std::this_thread::get_id();
			if(threadIDToDBPtrMap.find(theThreadID) ==  threadIDToDBPtrMap.end()) return false;
			return DatabaseInstanceManager::threadIDToDBPtrMap[theThreadID] != nullptr;
		}

		static inline void closeActiveDatabase() {
			std::lock_guard<std::mutex> theLG(DatabaseInstanceManager::databaseInstanceManagerLock);
			threadID theThreadID = std::this_thread::get_id();
			DatabaseInstanceManager::threadIDToDBPtrMap[theThreadID].reset();
			DatabaseInstanceManager::threadIDToDBPtrMap.erase(theThreadID);
		}

		static inline bool databaseExists(const std::string& aDatabaseName) {
			return FolderReader(Config::getStoragePath()).exists(aDatabaseName);
		}

		static void commit(){
			threadID theThreadID = std::this_thread::get_id();
			if(DatabaseInstanceManager::hasActiveDatabase()){
				DatabaseInstanceManager::threadIDToDBPtrMap[theThreadID]->releaseAllLocks();
			}
		}

		static void unRegister(){
			std::lock_guard<std::mutex> theLG(DatabaseInstanceManager::databaseInstanceManagerLock);
			threadID theThreadID = std::this_thread::get_id();
			DatabaseInstanceManager::threadIDToDBPtrMap[theThreadID]->releaseAllLocks();
			DatabaseInstanceManager::threadIDToDBPtrMap.erase(theThreadID);
		}

		static void abort(){
			threadID theThreadID = std::this_thread::get_id();
			if(DatabaseInstanceManager::hasActiveDatabase()){
				DatabaseInstanceManager::threadIDToDBPtrMap[theThreadID]->undoAllTransactionLog();
			}
		}

		static void setActiveDatabase(const std::string& aDatabaseName);
	};
}
#endif
