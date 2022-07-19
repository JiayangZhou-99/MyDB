//
//  Database.cpp
//

#include <string>
#include <iostream>
#include "Database.hpp"
#include "Config.hpp"
#include "Entity.hpp"
#include "QueryResultView.hpp"
#include "MetaHeaderBlock.hpp"
#include "TableView.hpp"
#include "Helpers.hpp"
#include "Alter.hpp"

namespace MyDB {
	Database::~Database() {
		//Datanase is being closed so we write the metadata to disk
		// std::cout << "database " << name << "deconstructed and saved" << std::endl;
		writeBlockUncached(metadata);
		writeLog(logmeta);
	}


	std::shared_ptr<LRUCache<uint64_t, BlockPtr>> Database::getBlockCache(){
		return blockCache;
	}

	void Database::updateCache(std::shared_ptr<LRUCache<uint64_t, BlockPtr>> anInputCache){
		blockCache = anInputCache;
	}

	std::shared_ptr<LockManager> Database::getLockManager(){ 
		return lockManagerPtr;
	}

	TransactionPtr               Database::getTransactionPtr(){
		return transactionShrPtr;
	}

	void Database::updateLockManager(std::shared_ptr<LockManager> anLockManagerPtr){
		lockManagerPtr = anLockManagerPtr;
	}

	void Database::updateMeta(){
		if (Config::getCacheSize(CacheType::block) > 0)
		{
			if(blockCache->contains(0)){
				BlockPtr theBlock;
				readBlock(0, theBlock);
				// MetaHeaderBlock* theMetaBlock = dynamic_cast<MetaHeaderBlock*>(theBlock.get());
				metadata = std::make_shared<MetaHeaderBlock>(theBlock); // a bug here!!!!!
			}else{
				BlockPtr theBlock;
				readBlockUncached(0, theBlock); 
				metadata = std::make_shared<MetaHeaderBlock>(theBlock);
				writeBlock(metadata);
			}
		}
	}

	/// <summary>
	/// Dumps the database to the specified stream
	/// </summary>
	/// <param name="anOutput">The output stream to use</param>
	/// <returns>The status of the dump operation</returns>
	void Database::dump(std::ostream& anOutput) {

		metadata->refreshState();
		auto theTableView = TableView("Type");
		int theRowAffected=0;

		eachBlock([&](BlockPtr const& aBlock){
			//Need to translate block type to string
			theTableView.addData(gBlockNames.at(aBlock->getBlockType()));
			theRowAffected += 1;
		});

		theTableView.show(anOutput);
		QueryResultView(theRowAffected).show(anOutput);
	}

	void Database::createTable(std::ostream& anOutput, EntityPtr& anEntityPtr) {
		
		lockManagerPtr->LockExclusive(transactionShrPtr.get(),0);
		transactionShrPtr->GetExclusiveLockSet()->emplace(0);
		FreeBlockPtr theFreeBlock;

		EntityBlockPtr theEntityBlock = std::make_shared<EntityBlock>();
		theEntityBlock->setEntity(anEntityPtr);

		popFreeBlock(theFreeBlock);

		//Setting up block header pointers and block index which is used
		//for writing the block
		theEntityBlock->setBlockIndex(theFreeBlock->getBlockIndex());
		theEntityBlock->nextEntityIndex = metadata->firstEntityIndex;
		metadata->firstEntityIndex = theEntityBlock->getBlockIndex();
		writeBlock(theEntityBlock);
		writeBlock(metadata);
		
		QueryResultView(1).show(anOutput);
	}

	void Database::describeTable(std::ostream& anOutput, std::string aTableName) {
		EntityBlockPtr theEntityBlock;
		size_t thePreviousEntityIndex;
		getEntityBlock(aTableName, theEntityBlock, thePreviousEntityIndex);

		auto theTableView = TableView("Field", "Type", "Null", "Key", "Default", "Extra");

		AttributeList theAttributeList = theEntityBlock->entity.get()->getAttributes();
		for (Attribute& theAttribute : theAttributeList) {
			std::string theExtraInfo = "";
			if (theAttribute.isAutoIncrement()) theExtraInfo += " auto_increment ";
			if (theAttribute.isPrimaryKey()) theExtraInfo += " primary key ";

			theTableView.addData(theAttribute.getName(),
				Helpers::dataTypeToString(theAttribute.getType(), theAttribute.getSize()),
				Helpers::boolToString(theAttribute.isNullable()),
				Helpers::boolToString(theAttribute.isPrimaryKey()),
				theAttribute.getDefaultString(),
				theExtraInfo
			);
		}

		theTableView.show(anOutput);
	}

	void Database::dropTable(std::ostream& anOutput, std::string aTableName) {
		
		lockManagerPtr->LockShared(transactionShrPtr.get(),0);
		transactionShrPtr->GetSharedLockSet()->emplace(0);

		EntityBlockPtr theEntityBlockToDelete;
		size_t thePreviousEntityIndex;
		size_t theNumBlocksAffected = 0;

		//We want the index of the entity before this entity in the list
		//because our list is singly linked and we need to point the previous
		//node to the node past the one we are deleting
		getEntityBlock(aTableName, theEntityBlockToDelete, thePreviousEntityIndex);

		//Pointing the previous entity to the next node if this table isn't
		//the head of the list
		if (thePreviousEntityIndex != Block::kEndOfList) {
			EntityBlockPtr thePreviousEntity;
			BlockPtr theBlock;
			readBlock(thePreviousEntityIndex, theBlock);
			thePreviousEntity = std::make_shared<EntityBlock>(theBlock);
			thePreviousEntity->nextEntityIndex = theEntityBlockToDelete->nextEntityIndex;
			writeBlock(thePreviousEntity);
		}
		else {
			//If this is the head, modify the metadata to point to the correct node
			metadata->firstEntityIndex = theEntityBlockToDelete->nextEntityIndex;
		}

		//Drop all associated data blocks
		eachDataOfEntity(theEntityBlockToDelete, [&](DataBlockPtr& aBlock)
			{
				pushFreeBlock(aBlock);
				++theNumBlocksAffected;
			});

		pushFreeBlock(theEntityBlockToDelete);
		++theNumBlocksAffected;

		QueryResultView(theNumBlocksAffected).show(anOutput);
	}

	void Database::showTables(std::ostream& anOutput) {
		
		
		////////////////// important here !!!!///////////////
		updateMeta();
		auto theTableView = TableView("Tables_in_" + name);
		eachEntity([&](EntityBlockPtr& anEntity)
			{
				theTableView.addData(anEntity->entity->getName());
			});

		theTableView.show(anOutput);
	}

	void Database::insert(std::ostream& anOutput, RowCollectionPtr& aCollectionOfRows, std::string aTableName) {

		lockManagerPtr->LockExclusive(transactionShrPtr.get(),0);
		transactionShrPtr->GetExclusiveLockSet()->emplace(0);

		TransactionLogBlock theCurTransactionLog;
		std::vector<BlockPtr> theInsertedBlockPtrs;
		// copy corrent meta
		DatabaseMetaHeaderBlockPtr theCopyMeta = std::make_shared<MetaHeaderBlock>(metadata);
		theInsertedBlockPtrs.push_back(theCopyMeta);

		EntityBlockPtr theEntityBlock;
		size_t thePreviousIndex;
		getEntityBlock(aTableName, theEntityBlock, thePreviousIndex);

		EntityBlockPtr theCopyEntity = std::make_shared<EntityBlock>(theEntityBlock);
		theInsertedBlockPtrs.push_back(theCopyEntity);

		//add a write lock to the entity and file metaheader
		lockManagerPtr->LockExclusive(transactionShrPtr.get(),theEntityBlock->getBlockIndex());
		transactionShrPtr->GetExclusiveLockSet()->emplace(theEntityBlock->getBlockIndex());

		AttributeList theAttributeList = theEntityBlock->entity.get()->getAttributes();

		//Making the schema a shared pointer because we want multiple row objects to have a reference to it
		std::shared_ptr<Entity> theSchema = std::make_shared<Entity>(*(theEntityBlock->entity));
		FreeBlockPtr theFreeBlock = std::make_shared<FreeBlock>();
		DataBlockPtr theDataBlock;
		
		OperationBlockPtr  theOperationBlock;
		lockManagerPtr->lockLog();
		
		for (Row theRow : *aCollectionOfRows) {

			theDataBlock = std::make_shared<DataBlock>(theSchema);
			theDataBlock->setData(theRow);
			popFreeBlock(theFreeBlock);
			theDataBlock->setBlockIndex(theFreeBlock->getBlockIndex());
			theDataBlock->nextDataIndex = theEntityBlock->firstDataIndex;
			
			//push to the in memory log
			theInsertedBlockPtrs.push_back(theDataBlock); 
			writeBlock(theDataBlock);

			//Need to track where the new head of the list is
			//Using it as a stack for constant time insert
			theEntityBlock->firstDataIndex = theDataBlock->getBlockIndex();
			theEntityBlock->entity->updateAutoIncrementValue();
		}

		writeBlock(theEntityBlock);
		theCurTransactionLog = TransactionLogBlock{OperationType::INSERT,theInsertedBlockPtrs};
		transactionLog.push_back(theCurTransactionLog);
		lockManagerPtr->unlockLog();

		QueryResultView(aCollectionOfRows->size()).show(anOutput);
	}

	void Database::select(std::ostream& anOutput, SQLQueryPtr& aSelectQueryPtr) {
		std::string   theEntityName = aSelectQueryPtr->getTableName();
		std::vector<std::unique_ptr<Row>> theRows;
		std::vector<int>                  theRowIndex;

		// adding share lock to the entity table
		EntityBlockPtr theEntityBlock;
		size_t thePreviousIndex;
		getEntityBlock(theEntityName, theEntityBlock, thePreviousIndex);
		lockManagerPtr->LockShared(transactionShrPtr.get(),theEntityBlock->getBlockIndex());
		transactionShrPtr->GetSharedLockSet()->emplace(theEntityBlock->getBlockIndex());

		if (aSelectQueryPtr->hasJoin) {
			doTableJoin(aSelectQueryPtr, theRows);
		}
		else {
			//move all the data records into theRows
			eachDataOfEntity(theEntityName, [&](DataBlockPtr& aDataBlockPtr) {
				theRows.push_back(std::make_unique<Row>(aDataBlockPtr->getData()));
				});
		}

		std::vector<std::string> theFields;
		if (aSelectQueryPtr->getTargets()[0] == "*") {
			//select all the data in the table
			for (auto theFieldKeyPair : theRows[0]->getData()) {
				theFields.push_back(theFieldKeyPair.first);
			}
		}
		else {
			//select according to the expression in SQLQuery
			for (auto theField : aSelectQueryPtr->getTargets()) {
				theFields.push_back(theField);
			}
		}

		//-------------------------  where conditions  -----------------------------//

		if (aSelectQueryPtr->hasWhere) {
			size_t theIndex = 0;
			while (theIndex < theRows.size()) {
				if (!aSelectQueryPtr->matches(theRows[theIndex]->getData())) {
					theRows.erase(theRows.begin() + theIndex);
				}
				else {
					theIndex += 1;
				}
			}
		}

		//-------------------------- order conditions -----------------------------//

		if (aSelectQueryPtr->ordered) {
			std::vector<std::string> theCompList = aSelectQueryPtr->getOrderFields(); //there can be several order fields
			Order  theOrderWay = aSelectQueryPtr->getOrderWay();
			if (theRows.size() == 0) throw Errors::unknownError;
			std::sort(theRows.begin(), theRows.end(), [&](std::unique_ptr<Row>& aRowA, std::unique_ptr<Row>& aRowB) {
				size_t theIndex = 0;

				while (theIndex < theCompList.size() &&
					Helpers::ValueToString(aRowA->getData().at(theCompList[theIndex])) == Helpers::ValueToString(aRowB->getData().at(theCompList[theIndex]))) {
					theIndex += 1;
				}

				if (theIndex >= theCompList.size()) return false;

				if (theOrderWay == Order::ASC) {
					return Helpers::ValueToString(aRowA->getData().at(theCompList[theIndex])) < Helpers::ValueToString(aRowB->getData().at(theCompList[theIndex]));
				}
				else {
					return Helpers::ValueToString(aRowA->getData().at(theCompList[theIndex])) > Helpers::ValueToString(aRowB->getData().at(theCompList[theIndex]));
				}
				});

		}

		//------------------------ limit conditions ---------------------------//

		if (aSelectQueryPtr->limited) {
			std::pair<int, int> theLimit = aSelectQueryPtr->getLimit();
			//first is count, second is start
			theRows.erase(theRows.begin(), theRows.begin() + theLimit.second);
			theRows.erase(theRows.begin() + theLimit.first, theRows.end());
		}

		auto theTableView = TableView(theFields);
		for (size_t theIndex = 0; theIndex < theRows.size(); theIndex++) {
			std::vector<std::string> theRowData;
			for (auto theKey : theFields) {
				KeyValues theMapKeyVal = theRows[theIndex]->getData();
				Value     theValue;
				if (theMapKeyVal.find(theKey) != theMapKeyVal.end()) {
					theValue = theMapKeyVal.at(theKey);
				}
				else {
					theValue = std::monostate();
				}
				theRowData.push_back(Helpers::ValueToString(theValue));
			}
			theTableView.addData(theRowData);
		}
		theTableView.show(anOutput);
	}


	void Database::deleteData(std::ostream& anOutput, SQLQueryPtr& aSelectQueryPtr) {

		lockManagerPtr->LockExclusive(transactionShrPtr.get(),0);
		transactionShrPtr->GetExclusiveLockSet()->emplace(0);

		TransactionLogBlock theCurTransactionLog;
		std::vector<BlockPtr> theDeletedBlockPtrs;

		DatabaseMetaHeaderBlockPtr theCopyMeta = std::make_shared<MetaHeaderBlock>(metadata);
		theDeletedBlockPtrs.push_back(theCopyMeta);

		size_t theNumBlocksAffected = 0;
		EntityBlockPtr theEntityBlock;

		getEntityBlock(aSelectQueryPtr->getTableName(), theEntityBlock);
		transactionShrPtr->GetExclusiveLockSet()->emplace(theEntityBlock->getBlockIndex());
		lockManagerPtr->LockExclusive(transactionShrPtr.get(),theEntityBlock->getBlockIndex());
		
		EntityBlockPtr theCopyEntity = std::make_shared<EntityBlock>(theEntityBlock);
		theDeletedBlockPtrs.push_back(theCopyEntity);

		theEntityBlock->entity->setToUpdateMode();

		Entity theUpdatedEntity = *theEntityBlock->entity.get();
		if (aSelectQueryPtr->hasWhere) {

			DataBlockPtr theLastBlock;

			eachDataOfEntity(aSelectQueryPtr->getTableName(), [&](DataBlockPtr& aDataBlockPtr) {
				aDataBlockPtr->setToUpdateMode();
				Row theRow = aDataBlockPtr->getData();
				if (aSelectQueryPtr->matches(theRow.getData())){

					theDeletedBlockPtrs.push_back(aDataBlockPtr);

					if (theLastBlock){
						theLastBlock->nextDataIndex = aDataBlockPtr->nextDataIndex;
						writeBlock(theLastBlock);
					}else{
						theEntityBlock->firstDataIndex = aDataBlockPtr->nextDataIndex;
						//                        writeBlock(theEntityBlock);
					}

					pushFreeBlock(aDataBlockPtr);
					++theNumBlocksAffected;
				}else{
					theLastBlock = std::move(aDataBlockPtr);
				}
			});
		}
		else{
			eachDataOfEntity(aSelectQueryPtr->getTableName(), [&](DataBlockPtr& aDataBlockPtr) {
				theDeletedBlockPtrs.push_back(aDataBlockPtr);
				pushFreeBlock(aDataBlockPtr);
				++theNumBlocksAffected;
			});
			theEntityBlock->firstDataIndex = Block::kEndOfList;
		}

		writeBlock(theEntityBlock);
		theCurTransactionLog = TransactionLogBlock{OperationType::DELETE,theDeletedBlockPtrs};
		transactionLog.push_back(theCurTransactionLog);
		QueryResultView(theNumBlocksAffected).show(anOutput);

	}

	void Database::updateData(std::ostream& anOutput, SQLQueryPtr& aSelectQueryPtr) {

		TransactionLogBlock theCurTransactionLog;
		std::vector<BlockPtr> theUpdatedBlockPtrs;

		int theNumBlocksAffected = 0;
		std::string   theEntityName = aSelectQueryPtr->getTableName();

		// adding share lock to the entity table
		EntityBlockPtr theEntityBlock;
		size_t thePreviousIndex;
		getEntityBlock(theEntityName, theEntityBlock, thePreviousIndex);

		lockManagerPtr->LockShared(transactionShrPtr.get(),theEntityBlock->getBlockIndex());
		transactionShrPtr->GetSharedLockSet()->emplace(theEntityBlock->getBlockIndex());

		std::pair<std::string, std::string> theUpdateInfo = aSelectQueryPtr->getUpdateInfo();
		eachDataOfEntity(theEntityBlock, [&](DataBlockPtr& aDataBlockPtr) {
			
			aDataBlockPtr->setToUpdateMode();
			Row theRow = aDataBlockPtr->getData();
			if (aSelectQueryPtr->matches(theRow.getData())) {
				//make a copy of the original data;
				theUpdatedBlockPtrs.push_back(std::make_shared<DataBlock>(*aDataBlockPtr));

				KeyValues& theKeyValue = theRow.getData();
				Value       theValue = theKeyValue[theUpdateInfo.first];
				Value       theUpdateVal = Helpers::StringToValue(theValue, theUpdateInfo.second);
				// have to update the data type from string to the original type
				theKeyValue[theUpdateInfo.first] = theUpdateVal;
				aDataBlockPtr->updateData(theRow);
				writeBlock(aDataBlockPtr);
				theNumBlocksAffected += 1;
			}

		});

		theCurTransactionLog = TransactionLogBlock{OperationType::UPDATE,theUpdatedBlockPtrs};
		transactionLog.push_back(theCurTransactionLog);

		QueryResultView(theNumBlocksAffected).show(anOutput);

	}


	void Database::showIndexing(std::ostream& anOutput) {
		// this is doing the show indexes
		auto theTableView = TableView("table", "field(s)");
		eachEntity([&](EntityBlockPtr& anEntityBlock) {
			AttributeList theAttributeList = anEntityBlock->entity.get()->getAttributes();
			std::string theEntityName = anEntityBlock->entity->getName();
			std::string theFields = "";
			for (Attribute& theAttribute : theAttributeList) {
				theFields += (theAttribute.getName() + " ");
			}
			theTableView.addData(theEntityName, theFields);
			});
		theTableView.show(anOutput);
	}

	void Database::getIndex(std::ostream& anOutput, IndexFieldsPtr& aFieldsPtr, std::string aTableName) {
		// this is doing the SHOW INDEX {field1,...} FROM {tablename}
		aFieldsPtr->push_back("block");
		auto theTableView = TableView(*aFieldsPtr.get());
		eachDataOfEntity(aTableName, [&](DataBlockPtr& aDataBlockPtr)
			{
				std::vector<std::string> theData;
				for (size_t theIndex = 0; theIndex < aFieldsPtr->size() - 1; theIndex++) {
					IndexFields     theNameOfField = *aFieldsPtr.get();
					Value           theValue = aDataBlockPtr->getData().getData().at(theNameOfField[theIndex]);
					theData.push_back(Helpers::ValueToString(theValue));
				}
				theData.push_back(std::to_string(aDataBlockPtr->getBlockIndex()));
				theTableView.addData(theData);
			});
		theTableView.show(anOutput);
	}

	//---------------------------------------------------------------------------------------------------------//

	void Database::doTableJoin(SQLQueryPtr& aSelectQueryPtr, std::vector<std::unique_ptr<Row>>& aRows) {

		std::string theLeftTable = aSelectQueryPtr->getTableName();
		JoinList    theJoinList = aSelectQueryPtr->getJoinConditions();
		std::string theRightTable = theJoinList[0].table;
		if (theJoinList[0].joinType == Keywords::right_kw) {
			std::swap(theLeftTable, theRightTable);
			for (size_t theIndex = 0; theIndex < theJoinList.size(); theIndex++) {
				std::swap(theJoinList[theIndex].expr.lhs, theJoinList[theIndex].expr.rhs);
			}
		}

		eachDataOfEntity(theLeftTable, [&](DataBlockPtr& aDataBlockPtr) {
			Row  theRowInLeft = aDataBlockPtr->getData();
			bool theFindResult = false;
			eachDataOfEntity(theRightTable, [&](DataBlockPtr& anOtherDataBlockPtr) {
				Row theRowInRight = anOtherDataBlockPtr->getData();
				bool theResult = true;
				for (auto theJoinCond : theJoinList) {
					if (!theJoinCond.expr(theRowInLeft.getData(), theRowInRight.getData())) {
						theResult = false;
					}
				}
				if (theResult) {
					theFindResult = true;
					Row theSumRow;  // add the left and right row together
					for (auto theKeyPair : theRowInLeft.getData()) {
						theSumRow.set(theKeyPair.first, theKeyPair.second);
					}
					for (auto theKeyPair : theRowInRight.getData()) {
						theSumRow.set(theKeyPair.first, theKeyPair.second);
					}
					aRows.push_back(std::make_unique<Row>(theSumRow));
				}
				});
			if (!theFindResult) {
				aRows.push_back(std::make_unique<Row>(theRowInLeft));
			}
			});

	}

    //----------------------------alter entity info ------------------------------------//


    void Database::alterEntityAddAttribute(std::ostream& anOutput,std::string anTableName, Attribute& anAtt){

		
        EntityBlockPtr theEntityBlock;
        getEntityBlock(anTableName,theEntityBlock);

		lockManagerPtr->LockExclusive(transactionShrPtr.get(),theEntityBlock->getBlockIndex());
		transactionShrPtr->GetExclusiveLockSet()->emplace(theEntityBlock->getBlockIndex());

        theEntityBlock->entity->addAttribute(anAtt);
        writeBlock(theEntityBlock);
        size_t theCount = 0;
        eachDataOfEntity(anTableName, [&](DataBlockPtr& aDataBlockPtr) {
            theCount += 1;
            aDataBlockPtr->setToUpdateMode();
//            Row theRow = aDataBlockPtr->getData();
//            Value theVal = anAtt.getDefault();
//            theRow.set(anAtt.getName(), theVal);
            writeBlock(aDataBlockPtr);
        });
        QueryResultView(theCount).show(anOutput);
        
    }
    void Database::alterEntityDropAttribute(std::ostream& anOutput,std::string anTableName,std::string anAttName){

        EntityBlockPtr theEntityBlock;
        getEntityBlock(anTableName,theEntityBlock);

		lockManagerPtr->LockExclusive(transactionShrPtr.get(),theEntityBlock->getBlockIndex());
		transactionShrPtr->GetExclusiveLockSet()->emplace(theEntityBlock->getBlockIndex());
		
        theEntityBlock->entity->removeAttribute(anAttName);
        writeBlock(theEntityBlock);
        size_t theCount = 0;
        eachDataOfEntity(anTableName, [&](DataBlockPtr& aDataBlockPtr) {
            theCount += 1;
            aDataBlockPtr->setToUpdateMode();
            writeBlock(aDataBlockPtr);
        });
        QueryResultView(theCount).show(anOutput);
    }

	void Database::releaseAllLocks(){

		transactionShrPtr->SetState(TransactionState::COMMITTED);
		
		// Perform all deletes before we commit.
		std::unordered_set<int> theLockSet;
		for (auto item : *transactionShrPtr->GetExclusiveLockSet()) {
			theLockSet.emplace(item);
		}
		for (auto item : *transactionShrPtr->GetSharedLockSet()) {
			theLockSet.emplace(item);
		}

		for (auto theBlockID : theLockSet) {
			lockManagerPtr->Unlock(transactionShrPtr.get(), theBlockID);
		}
		transactionShrPtr->clear();
	}

	//----------------------------------------------- database log related operations ------------------------------------------------------//

	void Database::undo(){

		lockManagerPtr->lockLog();
		// for the earlist version, can not redo anymore
		if(logmeta->currentCursorIndex == logmeta->kEndOfList) throw Errors::canNotUndoAnyMore;

		BlockPtr theCurrentBaseBlockPtr;
		readLogBlock(logmeta->currentCursorIndex,theCurrentBaseBlockPtr);
		OperationBlockPtr theCurOpBlock = std::shared_ptr<OperationBlock>(new OperationBlock(theCurrentBaseBlockPtr));

		BlockPtr thePrevBaseBlockPtr;
		readLogBlock(theCurOpBlock->prevOperationBlock,thePrevBaseBlockPtr);
		OperationBlockPtr thePrevOpBlock = std::shared_ptr<OperationBlock>(new OperationBlock(thePrevBaseBlockPtr));

		int theTotalNumberOfLogData = theCurOpBlock->getBlockIndex() - thePrevOpBlock->getBlockIndex();
		int numOfRowsAffected = 0;


		BlockPtr     theLastBlock;
		BlockPtr     theFirstBlock;
		BlockPtr     theCurBlock;
		DataBlockPtr theLastDataBlock;
		DataBlockPtr thefirstDataBlock;
		DataBlockPtr theCurDataBlock;

		switch (thePrevOpBlock->opType)
		{
		case OperationType::INSERT:
			readLogBlock(theCurOpBlock->getBlockIndex()-1,theLastBlock);
			readLogBlock(thePrevOpBlock->getBlockIndex()+1, theFirstBlock);
			thefirstDataBlock = std::shared_ptr<DataBlock>(new DataBlock(theLastBlock));
			theLastDataBlock = std::shared_ptr<DataBlock>(new DataBlock(theFirstBlock));

			eachEntity([&](EntityBlockPtr anEntityPtr){
				if(anEntityPtr->firstDataIndex == theLastDataBlock->getBlockIndex()){
					anEntityPtr->firstDataIndex = thefirstDataBlock->nextDataIndex;
					writeBlock(anEntityPtr);
				}
				for(int theIndex = theTotalNumberOfLogData - 1;theIndex>0;theIndex--){
					readLogBlock(theIndex,theCurBlock);
					theCurDataBlock = std::shared_ptr<DataBlock>(new DataBlock(theCurBlock));
					pushFreeBlock(theCurDataBlock);
					numOfRowsAffected += 1;
				}
				return;
			});
			/* code */
			break;
		case OperationType::DELETE:
			/* code */
			break;
		case OperationType::UPDATE:
			/* code */
			break;
		
		default:
			break;
		}
		lockManagerPtr->unlockLog();
	};


	void Database::redo(){
		lockManagerPtr->lockLog();
		// for the latest version, can not redo anymore
		if(logmeta->currentCursorIndex == logmeta->latestOperationBlockIndex) throw Errors::canNotRedoAnyMore;

		BlockPtr theCurrentBaseBlockPtr;
		readLogBlock(logmeta->currentCursorIndex,theCurrentBaseBlockPtr);
		OperationBlockPtr theCurOpBlock = std::shared_ptr<OperationBlock>(new OperationBlock(theCurrentBaseBlockPtr));

		BlockPtr theNextBaseBlockPtr;
		readLogBlock(theCurOpBlock->nextOperationBlock,theNextBaseBlockPtr);
		OperationBlockPtr theNextOpBlock = std::shared_ptr<OperationBlock>(new OperationBlock(theNextBaseBlockPtr));

		int theTotalNumberOfLogData = theNextOpBlock->getBlockIndex() - theCurOpBlock->getBlockIndex();
		int numOfRowsAffected = 0;

		BlockPtr     theLastBlock;
		BlockPtr     theFirstBlock;
		BlockPtr     theCurBlock;
		DataBlockPtr theLastDataBlock;
		DataBlockPtr thefirstDataBlock;
		DataBlockPtr theCurDataBlock;

		switch (theCurOpBlock->opType)
		{
		case OperationType::INSERT:
			/* code */
			break;
		case OperationType::DELETE:
			readLogBlock(theNextOpBlock->getBlockIndex()-1,theLastBlock);
			readLogBlock(theCurOpBlock->getBlockIndex()+1, theFirstBlock);
			thefirstDataBlock = std::shared_ptr<DataBlock>(new DataBlock(theLastBlock));
			theLastDataBlock = std::shared_ptr<DataBlock>(new DataBlock(theFirstBlock));

			eachEntity([&](EntityBlockPtr anEntityPtr){
				if(anEntityPtr->firstDataIndex == theLastDataBlock->getBlockIndex()){
					anEntityPtr->firstDataIndex = thefirstDataBlock->nextDataIndex;
					writeBlock(anEntityPtr);
				}
				for(int theIndex = theTotalNumberOfLogData - 1;theIndex>0;theIndex--){
					readLogBlock(theIndex,theCurBlock);
					theCurDataBlock = std::shared_ptr<DataBlock>(new DataBlock(theCurBlock));
					pushFreeBlock(theCurDataBlock);
					numOfRowsAffected += 1;
				}
				return;
			});
			break;
		case OperationType::UPDATE:
			/* code */
			break;
		
		default:
			break;
		}
		lockManagerPtr->unlockLog();
	};

	/*-------------------------------------------------------------------- In Memory Log --------------------------------------------------------------------*/
	// update operation only redo the corresponding datablocks, while insert and delete redo datablocks and metablock and entity block
	void Database::undoAllTransactionLog(){

		MetaHeaderBlock* 	theMeta;
		EntityBlock* 		theEntity;
		DataBlock* 			theDataBlock;

		for(auto& theTransactionLogBlock:transactionLog){
			switch (theTransactionLogBlock.first)
			{
			case OperationType::INSERT:
			case OperationType::DELETE:
				theMeta = dynamic_cast<MetaHeaderBlock*>(theTransactionLogBlock.second[0].get());
				writeBlock(theMeta);
				theEntity = dynamic_cast<EntityBlock*>(theTransactionLogBlock.second[1].get());
				writeBlock(theEntity);

				for(size_t theIndex=2;theIndex<theTransactionLogBlock.second.size();theIndex++){
					theDataBlock = dynamic_cast<DataBlock*>(theTransactionLogBlock.second[theIndex].get());
					writeBlock(theDataBlock);
				}
				break;
			case OperationType::UPDATE:
				for(auto& theBlock:theTransactionLogBlock.second){
					theDataBlock = dynamic_cast<DataBlock*>(theBlock.get());
					writeBlock(theDataBlock);
				}
				break;
			}
		}
	}

}
