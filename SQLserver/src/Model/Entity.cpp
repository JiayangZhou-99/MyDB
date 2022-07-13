//
//  Entity.cpp
//
//

#include <stdio.h>
#include <vector>
#include <optional>
#include <memory>
#include <string>
#include "Entity.hpp"

namespace MyDB {

	Entity::Entity(const std::string aName) :
		name(aName), autoincr(1) {}

	Entity::Entity(const Entity& aCopy) {
		name = aCopy.name;
		autoincr = aCopy.autoincr;
		attributes = aCopy.attributes;
	}

	Entity::~Entity() {
		//std::cout << "~Entity()\n";
	}

	Entity& Entity::addAttribute(const Attribute& anAttribute) {
		if (!getAttribute(anAttribute.getName())) {
			attributes.push_back(anAttribute);
		}
		return *this;
	}

	Attribute* Entity::getAttribute(const std::string& aName) {
		for (size_t theIndex = 0; theIndex < attributes.size(); ++theIndex)
		{
			if (attributes[theIndex].getName() == aName)
			{
				return &attributes[theIndex];
			}
		}
		return nullptr;
	}

	// USE: ask the entity for name of primary key (may not have one...)
	const Attribute* Entity::getPrimaryKey() const {
		return nullptr;
	}

	size_t Entity::getPrimaryKeyNumber() const {
		size_t theNumber = 0;
		for (size_t theI = 0; theI < attributes.size(); theI++) {
			if (attributes[theI].isPrimaryKey()) {
				theNumber += 1;
			}
		}
		return theNumber;
	}
}
