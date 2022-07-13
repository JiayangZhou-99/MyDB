#include "LogMetaBlock.hpp"

namespace MyDB
{
    LogMetaBlock::LogMetaBlock(const BlockPtr& aBlock){
        std::memcpy(rawBlockData, aBlock->rawBlockData, kBlockSize);
		refreshState();
    }

    void    LogMetaBlock::encode(std::fstream& anOutput){
        saveState();
		//Save the char array to disk
		anOutput.write(rawBlockData, kBlockSize);
		anOutput.flush();

		if (!anOutput.good())
		{
			throw writeError;
		}
    }

    void    LogMetaBlock::decode(std::fstream& anInput){
        //Read the char array from disk
		anInput.read(rawBlockData, kBlockSize);

		refreshState();

		if (!anInput.good())
		{
			throw readError;
		}
    }

    void    LogMetaBlock::refreshState(){

        bufferOffset = 0;
		SerializedBlockTypeType theBlockType;
		read(theBlockType);
		blockType = static_cast<BlockType>(theBlockType);

		read(databaseFormatVersion);
		read(latestOperationBlockIndex);
        read(currentCursorIndex);
		read(numBlocks);

    }
    void    LogMetaBlock::saveState(){

        bufferOffset = 0;
		//Serialize to internal char array
		write(static_cast<SerializedBlockTypeType>(blockType));
		write(databaseFormatVersion);
		write(latestOperationBlockIndex);
        write(currentCursorIndex);
		write(numBlocks);

    }
} // namespace MyDB
