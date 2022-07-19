//
//  Storage.cpp
//
//
//

#include "Storage.hpp"
#include "Config.hpp"
#include "Blocks.hpp"
#include "TableView.hpp"
#include "Helpers.hpp"
#include "QueryResultView.hpp"

#include <filesystem>
namespace fs = std::filesystem;

namespace MyDB {
	Storage::Storage(const std::string& aStorageName, AccessMode aMode) :
		blockCache(std::make_shared<LRUCache<uint64_t, BlockPtr>>(Config::getCacheSize(CacheType::block))),
		fullPath(Config::getDBPath(aStorageName)),
		name(aStorageName),
		databaseStorageStream(std::fstream(fullPath, static_cast<std::ios_base::openmode>(aMode))),
		logStorageStream(std::fstream(fullPath+".log", static_cast<std::ios_base::openmode>(aMode))),
		metadata(std::make_shared<MetaHeaderBlock>()),
		logmeta(std::make_shared<LogMetaBlock>()){
			// if (Config::getCacheSize(CacheType::block) > 0)
			// {
			// 	if(blockCache->contains(0)){
			// 		metadata = std::make_shared<MetaHeaderBlock>(blockCache->get(0));
			// 	}else{
			// 		BlockPtr theBlock = blockCache->put(metadata->getBlockIndex(),metadata);
			// 		writeBlockUncached(metadata);
			// 	}
			// }
			// writeLog(logmeta);
		}

	Storage::~Storage()
	{
		blockCache->flush([&](BlockPtr aBlock){
			writeBlockUncached(aBlock);
		});
		databaseStorageStream.close();
	}

	void Storage::readLogBlock(SerializedIndexPtrType aBlockIndex,BlockPtr& aBlock){

		aBlock = std::make_shared<Block>();
		logStorageStream.seekg(aBlockIndex * Block::kBlockSize);
		aBlock->decode(logStorageStream);
		if (!logStorageStream.good()) throw readError;
		
	}

	void Storage::readBlock(uint64_t aBlockIndex, BlockPtr& aBlock)
	{
		if (blockCache->contains(aBlockIndex))
		{
			aBlock = blockCache->get(aBlockIndex);
			return;
		}

		readBlockUncached(aBlockIndex, aBlock);

		aBlock->setBlockIndex(aBlockIndex);
		if (Config::getCacheSize(CacheType::block) > 0)
		{
			if (BlockPtr theBlock = blockCache->put(aBlockIndex, aBlock))
			{
				writeBlockUncached(theBlock);
			}
		}
	}

	void Storage::readBlockUncached(uint64_t aBlockIndex, BlockPtr& aBlock)
	{
		aBlock = std::make_shared<Block>();

		databaseStorageStream.seekg(aBlockIndex * Block::kBlockSize);
		aBlock->decode(databaseStorageStream);
		if (!databaseStorageStream.good()) throw readError;
	}

	void Storage::getEntityBlock(std::string anEntityName, EntityBlockPtr& aBlock)
	{
		size_t theDiscardedIndex;
		getEntityBlock(anEntityName, aBlock, theDiscardedIndex);
	}

	void Storage::getEntityBlock(std::string anEntityName, EntityBlockPtr& aBlock, size_t& aPreviousEntityBlock)
	{
		bool theTableWasFound = false;
		aPreviousEntityBlock = 0;
		eachEntityConditional([&](EntityBlockPtr& anEntityBlock)
			{
				if (anEntityBlock->entity->getName() == anEntityName)
				{
					theTableWasFound = true;
					aBlock = anEntityBlock;
					return true;
				}
				aPreviousEntityBlock = anEntityBlock->getBlockIndex();
				return false;
			});

		if (!theTableWasFound) throw unknownTable;
	}

	void Storage::popFreeBlock(FreeBlockPtr& aFreeBlock)
	{
		if (metadata->firstFreeBlockIndex == Block::kEndOfList) {
			// No free blocks in Database so we create a new one
			aFreeBlock = std::make_shared<FreeBlock>();
			aFreeBlock->setBlockIndex(metadata->numBlocks + 1);
			metadata->numBlocks += 1;
		}
		else {
			// Database has a free block
			BlockPtr theBlock;
			readBlock(metadata->firstFreeBlockIndex, theBlock);
			aFreeBlock = std::make_shared<FreeBlock>(theBlock);

			metadata->firstFreeBlockIndex = aFreeBlock->nextFreeBlockIndex;
		}
	}

	uint64_t Storage::getBlockCount() const
	{
		size_t theFileSize = fs::file_size(fullPath);
		return theFileSize / Block::kBlockSize;
	}

	void Storage::eachBlock(const BlockVisitor& aVisitor)
	{
		size_t theNumBlocks = getBlockCount();
		BlockPtr theBlock;
		for (size_t theBlockIndex = 0; theBlockIndex < theNumBlocks; ++theBlockIndex)
		{
			readBlock(theBlockIndex, theBlock);
			aVisitor(theBlock);
		}
	}

	void Storage::eachEntityConditional(const EntityBlockConditionalVisitor& aVisitor)
	{
		EntityBlockPtr theEntityBlock = std::make_shared<EntityBlock>();
		BlockPtr theBlock;
		size_t theEntityIndex = metadata->firstEntityIndex;
		bool theStopFlag = false;

		while (theEntityIndex != Block::kEndOfList) {
			readBlock(theEntityIndex, theBlock);
			theEntityBlock = std::make_shared<EntityBlock>(theBlock);

			theStopFlag = aVisitor(theEntityBlock);

			if (theStopFlag) return;

			theEntityIndex = theEntityBlock->nextEntityIndex;
		}
	}

	void Storage::eachEntity(const EntityBlockVisitor& aVisitor)
	{
		EntityBlockPtr theEntityBlock = std::make_shared<EntityBlock>();
		size_t theEntityIndex = metadata->firstEntityIndex;
		BlockPtr theBlock;

		while (theEntityIndex != Block::kEndOfList) {
			readBlock(theEntityIndex, theBlock);
			theEntityBlock = std::make_shared<EntityBlock>(theBlock);

			aVisitor(theEntityBlock);

			theEntityIndex = theEntityBlock->nextEntityIndex;
		}
	}

	void Storage::eachDataOfEntity(const std::string& anEntityName, const EntityDataVisitor& aVisitor)
	{
		EntityBlockPtr theEntity;

		size_t thePreviousEntityIndex;

		getEntityBlock(anEntityName, theEntity, thePreviousEntityIndex);

		eachDataOfEntity(theEntity, aVisitor);
	}

	void Storage::eachDataOfEntity(EntityBlockPtr& anEntity, const EntityDataVisitor& aVisitor)
	{
		std::shared_ptr<Entity> theSchema = std::make_shared<Entity>(*(anEntity->entity));
		DataBlockPtr theDataBlock;
		BlockPtr theBlock;

		size_t theBlockIndex = anEntity->firstDataIndex;
		while (theBlockIndex != Block::kEndOfList) {
			readBlock(theBlockIndex, theBlock);
			theDataBlock = std::shared_ptr<DataBlock>(new DataBlock(theBlock));
			theDataBlock->updateSchema(theSchema);
			theBlockIndex = theDataBlock->nextDataIndex;
			aVisitor(theDataBlock);
		}
	}
}
