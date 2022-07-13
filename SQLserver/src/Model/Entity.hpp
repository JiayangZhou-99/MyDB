//
//  Entity.hpp
//  Assignment3
//
//  Created by rick gessner on 3/18/22.
//  Copyright © 2022 rick gessner. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include <vector>
#include <optional>
#include <memory>
#include <string>

#include "Attribute.hpp"
#include "Errors.hpp"
#include "BasicTypes.hpp"

namespace MyDB {
	class Entity;
	using AttributeOpt = std::optional<Attribute>;
	using AttributeList = std::vector<Attribute>;
	using EntityPtr = std::unique_ptr<Entity>;
	using SharedEntityPtr = std::shared_ptr<Entity>;

	//------------------------------------------------

	class Entity {
	public:
		Entity(const std::string aName);
		Entity(const Entity& aCopy);

		~Entity();

		const std::string& 		getName() const { return name; }

		const AttributeList& 	getAttributes() const { return attributes; }
		Entity& 				addAttribute(const Attribute& anAttribute);
		Attribute* 				getAttribute(const std::string& aName);
		const Attribute* 		getPrimaryKey() const;
		size_t                	getPrimaryKeyNumber() const;
        
        void removeAttribute(std::string attName){
            for(int i =0 ;i< attributes.size();i++){
                if(attributes[i].getName() == attName){
                    attributes.erase(attributes.begin()+i);
                }
            }
        }

		int getNextAutoIncrementValue() {
			return autoincr++;
		}

		int getNextAutoIncrementValueForSerialization() {
			return autoincr;
		}

		void setAutoIncrementValue(uint64_t aValue) {
			autoincr = aValue;
		}

		void updateAutoIncrementValue() {
			autoincr++;
		}

		void setToUpdateMode() {
			for (size_t theIndex = 0; theIndex < attributes.size(); theIndex++) {
				attributes[theIndex].setUpdateMode(true);
			}
		}

	protected:

		AttributeList   attributes;
		std::string     name;
		int        autoincr;  //auto_increment
	};

}
#endif /* Entity_hpp */
