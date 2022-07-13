#include "EntityBlock.hpp"

namespace MyDB {
	EntityBlock::EntityBlock(const BlockPtr& aBlock) : Block(aBlock->getBlockType(), aBlock->getBlockIndex())
	{
		std::memcpy(rawBlockData, aBlock->rawBlockData, kBlockSize);
		refreshState();
	}

	void EntityBlock::encode(std::fstream& anOutput)
	{
		saveState();

		//Save the char array to disk
		anOutput.write(rawBlockData, kBlockSize);
		anOutput.flush();

		if (!anOutput.good()) throw writeError;
	}

	void EntityBlock::saveState()
	{
		//Serialize to internal char array
		bufferOffset = 0;

		write(static_cast<SerializedBlockTypeType>(blockType));
		write(nextEntityIndex);
		write(indexIndex);
		write(firstDataIndex);
		write(entity->getName());
		write(entity->getNextAutoIncrementValueForSerialization());
		write(static_cast<SerializedCountType>(entity->getAttributes().size()));

		for (Attribute theAttribute : entity->getAttributes()) {

			write(theAttribute.getName());
			write(static_cast<SerializedTypeType>(theAttribute.getType()));
			write(static_cast<SerializedBoolType>(theAttribute.isAutoIncrement()));
			write(static_cast<SerializedBoolType>(theAttribute.isPrimaryKey()));
			write(static_cast<SerializedBoolType>(theAttribute.isNullable()));
			write(static_cast<SerializedCountType>(theAttribute.getSize()));

			write(theAttribute.hasDefault());

			//Only write a value if there is a default value
			if (theAttribute.hasDefault()) {
				write(theAttribute.getDefault());
			}
		}
	}

	void EntityBlock::decode(std::fstream& anInput)
	{
		//Read the char array from disk
		anInput.read(rawBlockData, kBlockSize);

		if (!anInput.good()) throw readError;

		refreshState();
	}

	void EntityBlock::setEntity(EntityPtr& aEntityPtr) {
		entity = std::move(aEntityPtr);
		associatedEntity = entity->getName();
	}

	void EntityBlock::refreshState()
	{
		SerializedCountType theAttributeCount = 0;
		std::string theEntityName;
		bufferOffset = 0;

		SerializedBlockTypeType theBlockType;
		read(theBlockType);
		blockType = static_cast<BlockType>(theBlockType);

		read(nextEntityIndex);
		read(indexIndex);
		read(firstDataIndex);
		read(theEntityName);
		entity = std::make_unique<Entity>(theEntityName);
		associatedEntity = entity->getName();

		int theAutoIncrementValue;
		read(theAutoIncrementValue);
		entity->setAutoIncrementValue(theAutoIncrementValue);

		read(theAttributeCount);

		Attribute theAttribute;
		for (SerializedCountType theAttributeIndex = 0; theAttributeIndex < theAttributeCount; ++theAttributeIndex) {
			theAttribute = Attribute();

			std::string theAttributeName;
			read(theAttributeName);
			theAttribute.setName(theAttributeName);

			SerializedTypeType theAttributeType;
			read(theAttributeType);
			theAttribute.setDataType(static_cast<DataTypes>(theAttributeType));

			SerializedBoolType theAttributeBool;
			read(theAttributeBool);
			theAttribute.setAutoIncrement(static_cast<bool>(theAttributeBool));

			read(theAttributeBool);
			theAttribute.setPrimaryKey(static_cast<bool>(theAttributeBool));

			read(theAttributeBool);
			theAttribute.setNullable(static_cast<bool>(theAttributeBool));

			SerializedCountType theSize;
			read(theSize);
			theAttribute.setSize(theSize);

			//Checking if there is a default value
			read(theAttributeBool);

			if (theAttributeBool) {
				Value theDefaultValue;
				read(theDefaultValue, theAttribute.getType());
				theAttribute.setDefault(theDefaultValue);
			}

			entity->addAttribute(theAttribute);
		}
	}
}
