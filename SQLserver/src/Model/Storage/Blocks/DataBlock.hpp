#ifndef DATABLOCK_HPP
#define DATABLOCK_HPP

#include <memory>
#include <map>
#include "Block.hpp"
#include "BasicTypes.hpp"
#include "Entity.hpp"
#include "Row.hpp"

namespace MyDB {
	class DataBlock : public Block {
	public:

		DataBlock(SharedEntityPtr aSchema = nullptr) : Block(BlockType::data_block), schema(aSchema) {}
		DataBlock(const BlockPtr& aBlock);

		void  encode(std::fstream& anOutput) override;
		void  decode(std::fstream& anInput) override;
		void  refreshState() override;
		void  saveState() override;

		void  setSchema(SharedEntityPtr& aSchema);
        void  updateSchema(SharedEntityPtr& aSchema);
        void  setToUpdateMode();

		void  setData(Row aRow);
        void  updateData(Row aRow);
        Row   getData();

		SerializedIndexPtrType nextDataIndex = 0;

	protected:
		Row data;

		SharedEntityPtr schema = nullptr;

		void updateDataFormat();
	};

	using DataBlockPtr = std::shared_ptr<DataBlock>;
}

#endif // !DATABLOCK_HPP
