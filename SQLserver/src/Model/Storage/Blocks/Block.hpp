#ifndef STORABLE_HPP
#define STORABLE_HPP

#include <fstream>
#include <map>
#include <memory>
#include <cstring>
#include "Errors.hpp"
#include "BasicTypes.hpp"

namespace MyDB {
	enum class BlockType {
		meta_block = 'M',
		data_block = 'D',
		free_block = 'F',
		index_block = 'I',
		entity_block = 'E',
		//other types?
		unknown_block = 'U',
		logMetaBlock  = 'L',
		operationBlock = 'O'
	};

	const std::map<BlockType, std::string> gBlockNames = {
	  std::make_pair(BlockType::meta_block, "Meta Block"),
	  std::make_pair(BlockType::data_block, "Data Block"),
	  std::make_pair(BlockType::free_block, "Free Block"),
	  std::make_pair(BlockType::index_block, "Index Block"),
	  std::make_pair(BlockType::entity_block, "Entity Block"),
	  std::make_pair(BlockType::unknown_block, "Unknown Block"),
	  std::make_pair(BlockType::logMetaBlock, "LogMeta Block"),
	  std::make_pair(BlockType::operationBlock, "Operation Block")
	};

	//Using the bitflags as the value to avoid later logic checks
	enum class AccessMode {
		AsNew = std::fstream::binary | std::fstream::in | std::fstream::out | std::fstream::trunc,
		AsExisting = std::fstream::binary | std::fstream::in | std::fstream::out
	};

	class Block;
	using BlockPtr = std::shared_ptr<Block>;

	/// <summary>
	/// Marks a class as able to be serialized into a block
	/// </summary>
	class Block {
	public:
		Block() : blockType(BlockType::unknown_block) {}

		virtual ~Block() = default;

		virtual void encode(std::fstream& anOutput);
		virtual void decode(std::fstream& anInput);

		//Refreshes the variables based on the raw data array
		virtual void refreshState();

		//Saves variables to the raw data array
		virtual void saveState();

		BlockType getBlockType() {
			return blockType;
		}

		uint64_t getBlockIndex() {
			return blockIndex;
		}

		std::string getAssociatedEntity() {
			return associatedEntity;
		}

		void setAssociatedEntity(const std::string& anEntity) {
			associatedEntity = anEntity;
		}

		void setBlockIndex(uint64_t anIndex) {
			blockIndex = anIndex;
		}

		BlockPtr asBaseBlock()
		{
			BlockPtr theNewBlock = std::make_shared<Block>();
			std::memcpy(theNewBlock->rawBlockData, rawBlockData, kBlockSize);
			theNewBlock->blockIndex = blockIndex;
			theNewBlock->associatedEntity = associatedEntity;
			theNewBlock->bufferOffset = bufferOffset;
			theNewBlock->blockType = blockType;
			return theNewBlock;
		}

		static const size_t kBlockSize = 1024;

		/// <summary>
		/// The version of the file format of the written blocks. This will be used to ensure the
		/// databases we read in are of a compatible version
		/// </summary>
		static const uint64_t kFileFormatVersion = 3;

		char rawBlockData[kBlockSize] = {};

		/// <summary>
		/// Value that indicates the end of a linked list
		/// </summary>
		static const uint64_t kEndOfList = 0;

		BlockType blockType = BlockType::unknown_block;

		//Used internally to track block location in the database, not written to file
		uint64_t blockIndex = 0;

		//Internal read/write pointer when encoding/decoding the block
		size_t bufferOffset = 0;

		//Internal member to track what Entity this block belongs to
		std::string associatedEntity;

	protected:
		Block(BlockType aBlockType, uint64_t aBlockIndex = 0, std::string anEntity = "") : blockType(aBlockType),
			blockIndex(aBlockIndex), associatedEntity(anEntity) { }

		//Functions to help reading/writing to the internal buffer
		template<typename T>
		void write(T aValue);

		void write(const std::string& aString);

		void write(Value aValue, DataTypes aType);

		template<typename T>
		void read(T& aValue);

		void read(std::string& aString);

		void read(Value& aValue, DataTypes aType);
	};

	template<typename T>
	inline void Block::write(T aValue)
	{
		//We reinterpret cast to convert arbitrary datatypes to byte arrays then write into them
		std::memcpy(rawBlockData + bufferOffset, reinterpret_cast<char*>(&aValue), sizeof(aValue));
		bufferOffset += sizeof(aValue);
	}

	template<typename T>
	inline void Block::read(T& aValue)
	{
		//We reinterpret cast to convert arbitrary datatypes to byte arrays then write into them
		std::memcpy(reinterpret_cast<char*>(&aValue), rawBlockData + bufferOffset, sizeof(aValue));
		bufferOffset += sizeof(aValue);
	}
}
#endif // !STORABLE_HPP
