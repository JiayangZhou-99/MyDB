#ifndef ENTITYBLOCK_HPP
#define ENTITYBLOCK_HPP

#include <memory>
#include "Block.hpp"
#include "Entity.hpp"
#include "BasicTypes.hpp"

namespace MyDB {

	class EntityBlock : public Block {
	public:
		EntityBlock() : Block(BlockType::entity_block) {}
		EntityBlock(const BlockPtr& aBlock);
		
		void encode(std::fstream& anOutput) override;
		void decode(std::fstream& anInput) override;
		void refreshState() override;
		void  saveState() override;

        void setEntity(EntityPtr& aEntityPtr);

		SerializedIndexPtrType nextEntityIndex = 0;
		SerializedIndexPtrType indexIndex = 0;
		SerializedIndexPtrType firstDataIndex = 0;

		EntityPtr entity;
	};

	using EntityBlockPtr = std::shared_ptr<EntityBlock>;
}

#endif // !ENTITYBLOCK_HPP
