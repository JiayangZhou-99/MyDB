#ifndef METAHEADERBLOCK_HPP
#define METAHEADERBLOCK_HPP

#include <memory>
#include "Block.hpp"
#include "BasicTypes.hpp"

namespace MyDB {
	class MetaHeaderBlock : public Block
	{
	public:
		MetaHeaderBlock() : Block(BlockType::meta_block) { }
		MetaHeaderBlock(const BlockPtr& aBlock);

		void encode(std::fstream& anOutput) override;
		void decode(std::fstream& anInput) override;
		void refreshState() override;
		void saveState() override;

		SerializedCountType 	databaseFormatVersion = Block::kFileFormatVersion;
		SerializedIndexPtrType 	firstEntityIndex = 0;
		SerializedIndexPtrType 	firstFreeBlockIndex = 0;
		SerializedCountType 	numBlocks = 0;

	protected:
	};

	using DatabaseMetaHeaderBlockPtr = std::shared_ptr<MetaHeaderBlock>;
}
#endif
