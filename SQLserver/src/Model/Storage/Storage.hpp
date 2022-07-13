//
//  Storage.hpp
//
//
//

#ifndef Storage_hpp
#define Storage_hpp

#include <fstream>
#include <iostream>
#include <functional>
#include <memory>
#include "Errors.hpp"
#include "Blocks.hpp"
#include "Entity.hpp"
#include "LRUCache.hpp"
#include "Config.hpp"
#include "LogMetaBlock.hpp"

namespace MyDB {

	//tags for db-open modes...
	struct CreateDB {}; 
	struct OpenDB {};

	class Storage;
	using StoragePtr = std::unique_ptr<Storage>;

	template<typename T>
	using BlockVisitorOfType = std::function<void(T aBlock)>;

	using BlockVisitor = std::function<void(BlockPtr& aBlock)>;

	using EntityBlockVisitor = std::function<void(EntityBlockPtr& anEntityBlock)>;
	using EntityBlockConditionalVisitor = std::function<bool(EntityBlockPtr& anEntityBlock)>;
	using EntityDataVisitor = std::function<void(DataBlockPtr& aDataBlock)>;
	using EntityVisitor = std::function<void(BlockPtr& aDataBlock)>;

	/// <summary>
	/// Performs operations for helping manage storing data on disk
	/// </summary>
	class Storage {
	protected:
		std::shared_ptr<LRUCache<uint64_t, BlockPtr>> blockCache; //should be sharePtr

		std::string fullPath;

		std::string name;

		std::fstream databaseStorageStream;

		std::fstream logStorageStream;

		template <typename T>
		void writeBlock(T& aBlock)
		{
			if (Config::getCacheSize(CacheType::block) > 0)
			{
				aBlock->saveState();
				BlockPtr theBaseBlock = aBlock->asBaseBlock();
				if (BlockPtr theBlock = blockCache->put(aBlock->getBlockIndex(), theBaseBlock))
				{
					writeBlockUncached(theBlock);
				}
			}
			else
			{
				writeBlockUncached(aBlock);
			}
			
		}

		template <typename T>
		void writeBlockUncached(T& aBlock)
		{
			databaseStorageStream.seekp(aBlock->getBlockIndex() * Block::kBlockSize);
			aBlock->encode(databaseStorageStream);
		}

		template<typename T>
		void writeLog(T& aBlock)
		{
			logStorageStream.seekp(aBlock->getBlockIndex()*Block::kBlockSize);
			aBlock->encode(logStorageStream);
		}

		void readLogBlock(SerializedIndexPtrType aBlockIndex,BlockPtr& aBlock);

		void readBlock(uint64_t aBlockIndex, BlockPtr& aBlock);

		void readBlockUncached(uint64_t aBlockIndex, BlockPtr& aBlock);

		/// <summary>
		/// Returns a free block to write into
		/// </summary>
		/// <param name="aFreeBlock"></param>
		void popFreeBlock(FreeBlockPtr& aFreeBlock);

		/// <summary>
		/// Marks a block as free. Assumes the list it was
		/// removed from has already had its pointers rearranged
		/// to account for the removal of the block
		/// </summary>
		template<typename T>
		void pushFreeBlock(T& aNewFreeBlock) {
			auto theFreeBlock = std::make_unique<FreeBlock>();

			//Need to make sure all pointers are linked correctly on the free
			//block list
			theFreeBlock->setBlockIndex(aNewFreeBlock->getBlockIndex());
			theFreeBlock->nextFreeBlockIndex = metadata->firstFreeBlockIndex;
			metadata->firstFreeBlockIndex = aNewFreeBlock->getBlockIndex();
			writeBlock(theFreeBlock);
		}

		/// <summary>
		/// Gets the number of blocks in the storage file
		/// </summary>
		/// <returns>The number of blocks including the metadata header</returns>
		uint64_t getBlockCount() const;

		Storage(const std::string& aStorageName, AccessMode aMode);

		~Storage();

		/// <summary>
		/// Iterates over every block in the storage
		/// </summary>
		/// <param name="aVisitor">The visitor to call</param>
		void eachBlock(const BlockVisitor& aVisitor);
		
		/// <summary>
		/// Traverses over every entity in the database with a condition to stop traversal
		/// if signaled by the visitor
		/// </summary>
		/// <param name="aVisitor"></param>
		void eachEntityConditional(const EntityBlockConditionalVisitor& aVisitor);

		/// <summary>
		/// Traverses over every entity in the database
		/// </summary>
		void eachEntity(const EntityBlockVisitor& aVisitor);

		/// <summary>
		/// Finds the requested entity then traverses all associated blocks
		/// </summary>
		/// <param name="anEntityName"></param>
		/// <param name="aVisitor"></param>
		void eachDataOfEntity(const std::string& anEntityName, const EntityDataVisitor& aVisitor);

		/// <summary>
		/// Traverses all associated blocks linked to the provided entity block
		/// </summary>
		void eachDataOfEntity(EntityBlockPtr& anEntity, const EntityDataVisitor& aVisitor);
        
		/// <summary>
		/// Finds an entity block by name
		/// </summary>
		/// <param name="aPreviousEntityIndex">The index of the preceeding entity in the list</param>
		void getEntityBlock(std::string anEntityName, EntityBlockPtr& aBlock, size_t& aPreviousEntityIndex);

		/// <summary>
		/// Finds an entity block by name
		/// </summary>
		void getEntityBlock(std::string anEntityName, EntityBlockPtr& aBlock);

		/// <summary>
		/// Locally cache metadata block since it will be read from/written to frequently
		/// We write to disk when the object is destructed
		/// </summary>
		DatabaseMetaHeaderBlockPtr metadata;
		LogMetaHeaderBlockPtr      logmeta;
	};
}
#endif /* Storage_hpp */
