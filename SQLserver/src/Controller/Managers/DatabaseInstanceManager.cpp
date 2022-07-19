#include "DatabaseInstanceManager.hpp"

namespace MyDB {

	std::mutex DatabaseInstanceManager::databaseInstanceManagerLock;
	std::unordered_map<threadID,DatabasePtr> threadIDToDBPtrMap;

	void DatabaseInstanceManager::setActiveDatabase(const std::string& aDatabaseName)
	{

		std::lock_guard<std::mutex> theLG(DatabaseInstanceManager::databaseInstanceManagerLock);
		
		if (!databaseExists(aDatabaseName)) throw unknownDatabase;

		threadID theThreadID = std::this_thread::get_id();

		if(DatabaseInstanceManager::threadIDToDBPtrMap[theThreadID]){
			DatabaseInstanceManager::threadIDToDBPtrMap[theThreadID]->releaseAllLocks();
		}

		bool   theDBUsedByOtherThread=false;
		threadID theSchemaThread;
		for(std::unordered_map<threadID,DatabasePtr>::iterator it = threadIDToDBPtrMap.begin();it != threadIDToDBPtrMap.end();it++){
			if(it->first != theThreadID && aDatabaseName==it->second->getDatabaseName()){
				theDBUsedByOtherThread = true;
				theSchemaThread = it->first;
				break;
			}
		}

		//Current DB is already the requested db
		if (threadIDToDBPtrMap[theThreadID] && threadIDToDBPtrMap[theThreadID]->getDatabaseName() == aDatabaseName) return;

		threadIDToDBPtrMap[theThreadID] = std::make_unique<Database>(aDatabaseName, OpenDB());
		if (threadIDToDBPtrMap[theThreadID]==nullptr)
		{
			throw databaseOpenError;
		}

		if(theDBUsedByOtherThread){
			threadIDToDBPtrMap[theThreadID]->updateCache(threadIDToDBPtrMap[theSchemaThread]->getBlockCache());
			threadIDToDBPtrMap[theThreadID]->updateLockManager(threadIDToDBPtrMap[theSchemaThread]->getLockManager());
		}
		threadIDToDBPtrMap[theThreadID]->updateMeta();
		threadIDToDBPtrMap[theThreadID]->getLockManager()->registerTransaction(threadIDToDBPtrMap[theThreadID]->getTransactionPtr().get());

	}
	
}
