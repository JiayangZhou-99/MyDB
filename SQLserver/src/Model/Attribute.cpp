//
//  Attribute.cpp
//  Assignment4
//
//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#include <iostream>
#include "Attribute.hpp"
#include "BasicTypes.hpp"

namespace MyDB {

	Attribute::Attribute(DataTypes aType)
		: type(aType), size(0), autoIncrement(0), primary(0), nullable(1) {
		updateSizeBasedOnType();
	}

	Attribute::Attribute(std::string aName, DataTypes aType, uint32_t aSize) {
		name = aName;
		type = aType;
		size = aSize;
		autoIncrement = 0;
		primary = 0;
		nullable = 1;
		updateSizeBasedOnType();
	}

	Attribute::Attribute(const Attribute& aCopy) {
		name = aCopy.name;
		type = aCopy.type;
		size = aCopy.size;
		autoIncrement = aCopy.autoIncrement;
		primary = aCopy.primary;
		nullable = aCopy.nullable;
		defaultValue = aCopy.defaultValue;
		updateMode = aCopy.updateMode;
	}

	Attribute::~Attribute() {
	}

	Attribute& Attribute::setName(std::string& aName) {
		name = aName;
		return *this;
	}

	Attribute& Attribute::setDataType(DataTypes aType) {
		type = aType;
		updateSizeBasedOnType();
		return *this;
	}

	Attribute& Attribute::setSize(int aSize) {
		size = aSize;
		return *this;
	}

	Attribute& Attribute::setAutoIncrement(bool anAuto) {
		autoIncrement = anAuto;
		return *this;
	}

	Attribute& Attribute::setPrimaryKey(bool aPrimary) {
		primary = aPrimary;
		return *this;
	}

	Attribute& Attribute::setNullable(bool aNullable) {
		nullable = aNullable;
		return *this;
	}

	Attribute& Attribute::setDefault(std::string aDefaultValueStr) {
		defaultValue = toValue(aDefaultValueStr);
		return *this;
	}

	void Attribute::updateSizeBasedOnType()
	{
		switch (type) {
		case DataTypes::int_type:
			size = sizeof(SerializedIntType);
			break;
		case DataTypes::float_type:
			size = sizeof(SerializedFloatType);
			break;
		case DataTypes::bool_type:
			size = sizeof(SerializedBoolType);
			break;
		case DataTypes::datetime_type:
			size = sizeof(SerializedDateTimeType);
			break;
		default:
			break;
		}
	}

	Attribute& Attribute::setDefault(Value aValue)
	{
		defaultValue = aValue;
		return *this;
	}

	bool Attribute::isValid() {
		return true;
	}

	Value Attribute::toValue(const std::string& aValue) const {
		Value theValue;
		switch (type) {
		case DataTypes::int_type:
			theValue = std::stoi(aValue);
			break;
		case DataTypes::float_type:
			theValue = std::stof(aValue);
			break;
		case DataTypes::bool_type:
			if (aValue == "false") theValue = false;
			if (aValue == "true") theValue = true;
			break;
		case DataTypes::datetime_type:
			//TODO: Fix datetime
			theValue = 0;
			break;
		case DataTypes::varchar_type:
			theValue = aValue;
			break;
		default:
			break;
		}
		return theValue;
	}

	std::string Attribute::getDefaultString() const {
		size_t theIndex = defaultValue.index();
		std::vector<std::string> theVector = { "NULL", "uint_64", "bool", "int", "float", "string" };
		if (theVector[theIndex] == "NULL")   return "NULL";
		if (theVector[theIndex] == "uint_64") return std::to_string(std::get<uint64_t>(defaultValue));

		if (theVector[theIndex] == "bool") {
			if (std::get<bool>(defaultValue) == true) {
				return "TRUE";
			}
			return "FALSE";
		}

		if (theVector[theIndex] == "int")    return std::to_string(std::get<int>(defaultValue));
		if (theVector[theIndex] == "float")  return std::to_string(std::get<double>(defaultValue));
		if (theVector[theIndex] == "string") return std::get<std::string>(defaultValue);
		return "NULL";
	}
}
