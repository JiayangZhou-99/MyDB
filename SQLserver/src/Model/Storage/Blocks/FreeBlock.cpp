#include "FreeBlock.hpp"

namespace MyDB {
	FreeBlock::FreeBlock(const BlockPtr& aBlock) : Block(aBlock->getBlockType(), aBlock->getBlockIndex())
	{
		std::memcpy(rawBlockData, aBlock->rawBlockData, kBlockSize);
		refreshState();
	}

	void FreeBlock::encode(std::fstream& anOutput)
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

	void FreeBlock::saveState()
	{
		//Serialize to internal char array
		bufferOffset = 0;
		write(static_cast<SerializedBlockTypeType>(blockType));
		write(nextFreeBlockIndex);
	}

	void FreeBlock::decode(std::fstream& anInput)
	{
		anInput.read(rawBlockData, kBlockSize);

		refreshState();

		if (!anInput.good())
		{
			throw readError;
		}
	}

	void FreeBlock::refreshState()
	{
		//Deserialize from internal char array
		bufferOffset = 0;
		SerializedBlockTypeType theBlockType;
		read(theBlockType);
		blockType = static_cast<BlockType>(theBlockType);

		read(nextFreeBlockIndex);
	}
}
