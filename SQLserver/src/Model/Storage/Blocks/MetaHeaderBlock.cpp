#include "MetaHeaderBlock.hpp"

namespace MyDB {
	MetaHeaderBlock::MetaHeaderBlock(const BlockPtr& aBlock) : Block(aBlock->getBlockType(), aBlock->getBlockIndex())
	{
		std::memcpy(rawBlockData, aBlock->rawBlockData, kBlockSize);
		refreshState();
	}

	void MetaHeaderBlock::encode(std::fstream& anOutput)
	{
		saveState();

		//Save the char array to disk
		anOutput.write(rawBlockData, kBlockSize);
		anOutput.flush();

		if (!anOutput.good())
		{
			throw writeError;
		}
	}

	void MetaHeaderBlock::saveState()
	{
		bufferOffset = 0;

		//Serialize to internal char array
		write(static_cast<SerializedBlockTypeType>(blockType));
		write(databaseFormatVersion);
		write(firstEntityIndex);
		write(firstFreeBlockIndex);
		write(numBlocks);
	}

	void MetaHeaderBlock::decode(std::fstream& anInput)
	{
		//Read the char array from disk
		anInput.read(rawBlockData, kBlockSize);

		refreshState();

		if (!anInput.good())
		{
			throw readError;
		}
		
	}

	void MetaHeaderBlock::refreshState()
	{
		bufferOffset = 0;
		SerializedBlockTypeType theBlockType;
		read(theBlockType);
		blockType = static_cast<BlockType>(theBlockType);

		read(databaseFormatVersion);
		read(firstEntityIndex);
		read(firstFreeBlockIndex);
		read(numBlocks);
	}
}
