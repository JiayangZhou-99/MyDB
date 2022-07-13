#include "OperationBlock.hpp"

namespace MyDB
{

    OperationBlock::OperationBlock(const BlockPtr& aBlock){
        std::memcpy(rawBlockData, aBlock->rawBlockData, kBlockSize);
		refreshState();
    }

    void    OperationBlock::encode(std::fstream& anOutput){
        saveState();
		//Save the char array to disk
		anOutput.write(rawBlockData, kBlockSize);
		anOutput.flush();

		if (!anOutput.good())
		{
			throw writeError;
		}
    }

    void    OperationBlock::decode(std::fstream& anInput){
        //Read the char array from disk
		anInput.read(rawBlockData, kBlockSize);

		refreshState();

		if (!anInput.good())
		{
			throw readError;
		}
    }

    void    OperationBlock::refreshState(){
        
        bufferOffset = 0;
		SerializedBlockTypeType theBlockType;
		read(theBlockType);
		blockType = static_cast<BlockType>(theBlockType);
		read(prevOperationBlock);
        read(nextOperationBlock);
        read(opType);
        opType = static_cast<OperationType>(opType);

    }

    void    OperationBlock::saveState(){
        bufferOffset = 0;
		//Serialize to internal char array
		write(static_cast<SerializedBlockTypeType>(blockType));
		write(prevOperationBlock);
        write(nextOperationBlock);
        write(static_cast<SerializedBlockTypeType>(blockType));
    }
    
} // namespace MyDB
