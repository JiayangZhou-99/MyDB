#include "Block.hpp"
#include "Errors.hpp"

namespace MyDB {
	void Block::encode(std::fstream& anOutput)
	{
		//Write the char array to disk
		anOutput.write(rawBlockData, kBlockSize);
		anOutput.flush();

		if (!anOutput.good()) throw writeError;
	}

	void Block::saveState()
	{

	}

	void Block::decode(std::fstream& anInput)
	{
		//Read the char array from disk
		anInput.read(rawBlockData, kBlockSize);

		refreshState();

		if (!anInput.good()) throw readError;
	}

	void Block::refreshState() {
		SerializedBlockTypeType theBlockType;
		read(theBlockType);
		blockType = static_cast<BlockType>(theBlockType);
	}

	void Block::read(std::string& aString)
	{
		SerializedCountType theLength;
		read(theLength);
		aString = std::string(&rawBlockData[bufferOffset], &rawBlockData[bufferOffset] + theLength);
		bufferOffset += theLength;
	}

	void Block::read(Value& aValue, DataTypes aType)
	{
		switch (aType) {
		case DataTypes::datetime_type:
		{
			SerializedDateTimeType theDate;
			read(theDate);
			aValue = theDate;
			break;
		}
		case DataTypes::bool_type:
		{
			bool theBool;
			read(theBool);
			aValue = theBool;
			break;
		}
		case DataTypes::float_type:
		{
			SerializedFloatType theFloat;
			read(theFloat);
			aValue = theFloat;
			break;
		}
		case DataTypes::int_type:
		{
			SerializedIntType theInt;
			read(theInt);
			aValue = theInt;
			break;
		}
		case DataTypes::varchar_type:
		{
			std::string theString;
			read(theString);
			aValue = theString;
			break;
		}
		default:
		{
			break;
		}
		}
	}

	void Block::write(const std::string& aString)
	{
		SerializedCountType theLength = aString.size();
		write(theLength);
		std::memcpy(rawBlockData + bufferOffset, aString.c_str(), aString.size());
		bufferOffset += aString.size();
	}

	void Block::write(Value aValue, DataTypes aType)
	{
		switch (aType) {
		case DataTypes::datetime_type:
		{
			SerializedDateTimeType theDate = std::get<uint64_t>(aValue);
			write(theDate);
			break;
		}
		case DataTypes::bool_type:
		{
			SerializedBoolType theBool = std::get<bool>(aValue);
			write(theBool);
			break;
		}
		case DataTypes::float_type:
		{
			SerializedFloatType theFloat = std::get<double>(aValue);
			write(theFloat);
			break;
		}
		case DataTypes::int_type:
		{
			SerializedIntType theInt = std::get<int>(aValue);
			write(theInt);
			break;
		}
		case DataTypes::varchar_type:
		{
			std::string theString = std::get<std::string>(aValue);
			write(theString);
			break;
		}
		default:
			break;
		}
	}
}
