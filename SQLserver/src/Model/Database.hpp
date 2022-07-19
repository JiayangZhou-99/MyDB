//
//  Database.hpp
//
//
//

#ifndef Database_hpp
#define Database_hpp

#include <fstream>
#include <memory>
#include "Storage.hpp"
#include "Entity.hpp"
#include "Row.hpp"
#include "SQLQuery.hpp"
#include "IndexStatement.hpp"
#include "Alter.hpp"
#include "LockManager.hpp"
#include "OperationBlock.hpp"


namespace MyDB {

	class Database;
	using DatabasePtr = std::unique_ptr<Database>;
	using DatabaseRawPtr = Database*;
	using TransactionLogBlock = std::pair<OperationType,std::vector<BlockPtr>>;

	class Database : public Storage {
	public:

		Database(const std::string& aName, CreateDB) : Storage(aName, AccessMode::AsNew), name(aName), changed(true) {
			transactionShrPtr = std::make_shared<Transaction>(TransactionState::GROWING,IsolationLevel::REPEATABLE_READ,std::this_thread::get_id());
			lockManagerPtr    =  std::make_shared<LockManager>();
		}

		Database(const std::string& aName, OpenDB) : Storage(aName, AccessMode::AsExisting), name(aName), changed(false) {
			
			//Load metadata block into database memory
			// BlockPtr theBlock;
			// readBlockUncached(0, theBlock); // bug here !!!!!!!!!!!!!!!!!!!!!!!!!
			// metadata = std::make_shared<MetaHeaderBlock>(theBlock);

			// metadata = std::make_shared<MetaHeaderBlock>();

			BlockPtr theLogMetaBlock;
			readLogBlock(0,theLogMetaBlock);
			logmeta = std::make_shared<LogMetaBlock>(theLogMetaBlock);

			transactionShrPtr = std::make_shared<Transaction>(TransactionState::GROWING,IsolationLevel::REPEATABLE_READ,std::this_thread::get_id());
			lockManagerPtr    =  std::make_shared<LockManager>();
		}

		virtual ~Database();

		std::shared_ptr<LRUCache<uint64_t, BlockPtr>> getBlockCache();

		void updateCache(std::shared_ptr<LRUCache<uint64_t, BlockPtr>> anInputCache);

		std::shared_ptr<LockManager> getLockManager();
		TransactionPtr               getTransactionPtr();

		void updateLockManager(std::shared_ptr<LockManager> anLockManagerPtr);

		void updateMeta();

		void dump(std::ostream& anOutput);

		void createTable(std::ostream& anOutput, EntityPtr& anEntityPtr);

		void showTables(std::ostream& anOutput);

		void describeTable(std::ostream& anOutput, std::string aTableName);

		void dropTable(std::ostream& anOutput, std::string aTableName);

		void insert(std::ostream& anOutput, RowCollectionPtr& aCollectionOfRows, std::string aTableName);

		void select(std::ostream& anOutput, SQLQueryPtr& aSelectQueryPtr);

		void deleteData(std::ostream& anOutput, SQLQueryPtr& aSelectQueryPtr);

		void updateData(std::ostream& anOutput, SQLQueryPtr& aSelectQueryPtr);
        
        void showIndexing(std::ostream& anOutput);
        
        void getIndex(std::ostream& anOutput, IndexFieldsPtr& aFieldsPtr, std::string aTableName);
        
		std::string getDatabaseName() const { return name; }
        
        void alterEntityAddAttribute(std::ostream& anOutput,std::string anTableName, Attribute& anAtt);

        void alterEntityDropAttribute(std::ostream& anOutput,std::string anTableName,std::string anAttName);

		// roll back the former operation, which also need lock_manager for synchronization
		void undo();
		void redo();

		//undo the log of current transaction when aborted
		void undoAllTransactionLog(); 

		//release all the lock after commited
		void releaseAllLocks();


	protected:
        
        void doTableJoin(SQLQueryPtr& aSelectQueryPtr,std::vector<std::unique_ptr<Row>>& aRows);
        
		std::string 	name;
		bool 			changed;
		std::shared_ptr<LockManager>      	lockManagerPtr = nullptr;
		TransactionPtr						transactionShrPtr = nullptr;

		//metain a in memory transaction log, each logblock contains operation type and blockPtr
		std::vector<TransactionLogBlock>    transactionLog;

	};
}
#endif /* Database_hpp */
