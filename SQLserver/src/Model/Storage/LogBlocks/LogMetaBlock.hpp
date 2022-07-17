#ifndef LOGMETABLOCK_HPP
#define LOGMETABLOCK_HPP

#include <memory>
#include "Block.hpp"
#include "BasicTypes.hpp"

namespace MyDB
{

    class LogMetaBlock: public Block{

    public:
        LogMetaBlock(): Block(BlockType::logMetaBlock){};
        LogMetaBlock(const BlockPtr& aBlock);
        ~LogMetaBlock()=default;

        void    encode(std::fstream& anOutput) override;
		void    decode(std::fstream& anInput) override;
		void    refreshState() override;
		void    saveState() override;

        SerializedCountType     databaseFormatVersion = Block::kFileFormatVersion;
		SerializedIndexPtrType  latestOperationBlockIndex = 0;
        SerializedIndexPtrType  currentCursorIndex = 0;
		SerializedCountType     numBlocks = 0;

    };
           
    using LogMetaHeaderBlockPtr = std::shared_ptr<LogMetaBlock>;
    
} // namespace MyDB

#endif