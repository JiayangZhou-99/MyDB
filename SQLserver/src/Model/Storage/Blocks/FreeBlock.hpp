#ifndef FREEBLOCK_HPP
#define FREEBLOCK_HPP

#include <memory>
#include "Block.hpp"
#include "BasicTypes.hpp"

namespace MyDB{
    class FreeBlock : public Block {
    public:
		FreeBlock() : Block(BlockType::free_block) { }
        FreeBlock(const BlockPtr& aBlock);

        void  encode(std::fstream& anOutput) override;
        void  decode(std::fstream& anInput) override;
        void refreshState() override;
        void  saveState() override;

		SerializedIndexPtrType nextFreeBlockIndex = 0;
	protected:

    };

    using FreeBlockPtr = std::shared_ptr<FreeBlock>;
}

#endif // !FREEBLOCK_HPP
