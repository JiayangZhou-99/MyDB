#ifndef OPERATIONBLOCK_HPP
#define OPERATIONBLOCK_HPP

#include "Block.hpp"

namespace MyDB{
    //for delete record: copy the data block record to log
        //undo: insert back and updat the data block in log
        //redo: delete base on block index

    //for insert record:
        //undo: delete base on block index 
        //redo: insert back and updat the data block in log

    //for update record:
        //undo: swap log data block and db data block
        //redo: swap log data block and db data block
    
    enum class OperationType{DELETE = 0,INSERT = 1,UPDATE = 2};

    class OperationBlock: public Block
    {
    public:
        OperationBlock(OperationType anOpType): Block(BlockType::operationBlock),opType(anOpType){}
        OperationBlock(const BlockPtr& aBlock);
        ~OperationBlock()=default;

        void    encode(std::fstream& anOutput) override;
		void    decode(std::fstream& anInput) override;
		void    refreshState() override;
		void    saveState() override;

        SerializedIndexPtrType      prevOperationBlock;
        SerializedIndexPtrType      nextOperationBlock;
        OperationType               opType;
    };

    using OperationBlockPtr = std::shared_ptr<OperationBlock>;

}


#endif