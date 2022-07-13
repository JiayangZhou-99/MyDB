#include "DataBlock.hpp"

namespace MyDB {
	DataBlock::DataBlock(const BlockPtr& aBlock) : Block(aBlock->getBlockType(), aBlock->getBlockIndex())
	{
		std::memcpy(rawBlockData, aBlock->rawBlockData, kBlockSize);
		refreshState();
	}

	void DataBlock::encode(std::fstream& anOutput)
	{
		saveState();

		//Save the char array to disk
		anOutput.write(rawBlockData, kBlockSize);
		anOutput.flush();

		if (!anOutput.good()) throw writeError;
	}

	void DataBlock::saveState()
	{
		bufferOffset = 0;
		write(static_cast<SerializedBlockTypeType>(blockType));

		write(nextDataIndex);

		//Note: We write a bool to indicate the data is not null
		for (Attribute theAttribute : schema->getAttributes()) {
			//Auto-Increment takes precedence
			if (!theAttribute.isUpdate() && theAttribute.isAutoIncrement()) {
				write(false, DataTypes::bool_type);
				write(schema->getNextAutoIncrementValue(), theAttribute.getType());
				continue;
			}

			//Write if data provided
			if (data.getData().find(theAttribute.getName()) != data.getData().end()) {
				Value theValue = data.getData().at(theAttribute.getName());
                if(theValue!= Value(std::monostate())){
                    write(false, DataTypes::bool_type);
                    write(theValue, theAttribute.getType());
                }else{
                    write(true, DataTypes::bool_type);
                }
				continue;
			}

			//No data provided so we check if there is a default available to write
			if (theAttribute.hasDefault()) {
				write(false, DataTypes::bool_type);
				write(theAttribute.getDefault());
				continue;
			}

			//No default either so we see if it can be null
			if (theAttribute.isNullable()) {
				//Null so we write true to the null flag
				write(true, DataTypes::bool_type);
				continue;
			}

			throw valueExpected;
		}
	}

	void DataBlock::decode(std::fstream& anInput)
	{
		//Read the char array from disk
		anInput.read(rawBlockData, kBlockSize);


		if (!anInput.good()) throw readError;
		
		refreshState();
	}

	void DataBlock::refreshState()
	{
		bufferOffset = 0;

		if (schema) associatedEntity = schema->getName();
		
		SerializedBlockTypeType theBlockType;
		read(theBlockType);
		blockType = static_cast<BlockType>(theBlockType);

		read(nextDataIndex);

		data.clearData();
		Value theValue;
		SerializedBoolType theNullFlag;

		if (schema) {
			for (Attribute theAttribute : schema->getAttributes()) {
				read(theNullFlag);
				if (theNullFlag) {
					theValue = std::monostate();
				}
				else {
					read(theValue, theAttribute.getType());
				}
				data.set(theAttribute.getName(), theValue);
			}
		}
	}

	void DataBlock::setSchema(SharedEntityPtr& aSchema)
	{
		if (schema) return;

		schema = aSchema;

		updateDataFormat();
	}

    void DataBlock::updateSchema(SharedEntityPtr& aSchema){
        
		schema = aSchema;

		refreshState();
    }

	void DataBlock::setData(Row aRow)
	{
		data = aRow;
		updateDataFormat();
	}

    void DataBlock::updateData(Row aRow)
    {
        data = aRow;
    }

    Row   DataBlock::getData(){
        return data;
    }

	void DataBlock::updateDataFormat()
	{
		//Need to update types based on schema
		for (auto& theData : data.getData())
		{
			Attribute* theAttribute = schema->getAttribute(theData.first);
			if (theAttribute) {
				std::string theString = std::get<std::string>(theData.second);
				Value theValue = theAttribute->toValue(theString);
				data.set(theData.first, theValue);
			}
		}
	}

    void  DataBlock::setToUpdateMode(){
        schema->setToUpdateMode();
    }

}
