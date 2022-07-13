//
//  Row.cpp
//
//
//

#include "Row.hpp"

namespace MyDB {

	Row::Row() {}

	Row::~Row() {}

	Row& Row::operator=(const Row& aRow) {
		data.clear();
		data.insert(aRow.data.begin(), aRow.data.end());
		return *this;
	}

	bool Row::operator==(Row& aCopy) const { return false; }

	Row& Row::set(const std::string& aKey, Value& aValue) {
		data[aKey] = aValue;
		return *this;
	}

}
