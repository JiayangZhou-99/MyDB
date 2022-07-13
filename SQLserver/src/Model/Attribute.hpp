//
//  Attribute.hpp
//
//
//

#ifndef Attribute_hpp
#define Attribute_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <optional>
#include <functional>
#include "keywords.hpp"
#include "BasicTypes.hpp"

namespace MyDB {

	class Attribute {
	protected:
		std::string   name;
		DataTypes     type;
		uint16_t      size : 10; //max=1000
		uint16_t      autoIncrement : 1;
		uint16_t      primary : 1;
		uint16_t      nullable : 1;
		Value         defaultValue = std::monostate{};
		bool          updateMode = false;

	public:

		Attribute(DataTypes aType = DataTypes::no_type);
		Attribute(std::string aName, DataTypes aType, uint32_t aSize = 0);
		Attribute(const Attribute& aCopy);
		~Attribute();

		Attribute& setName(std::string& aName);
		Attribute& setDataType(DataTypes aType);
		Attribute& setSize(int aSize);
		Attribute& setAutoIncrement(bool anAuto);
		Attribute& setPrimaryKey(bool anAuto);
		Attribute& setNullable(bool aNullable);
		Attribute& setDefault(std::string aDefaultValueStr);
		Attribute& setDefault(Value aValue);
		void       setUpdateMode(bool aMode) { updateMode = true; }

		bool                isValid(); //is this Entity valid?
		bool                isUpdate() { return updateMode; }
		std::string         getDefaultString() const;
		const std::string&  getName() const { return name; }
		DataTypes           getType() const { return type; }
		size_t              getSize() const { return size; }
		Value               getDefault() const { return defaultValue; }

		bool				hasDefault() const {
			try {
				//Monostate indicates no element
				std::get<std::monostate>(defaultValue);
				return false;
			}
			catch (std::bad_variant_access const& theException) {
				return true;
			}
		}

		Value               toValue(const std::string& aValue) const;

		bool                isPrimaryKey() const { return primary; }
		bool                isNullable() const { return nullable; }
		bool                isAutoIncrement() const { return autoIncrement; }

	protected:
		void updateSizeBasedOnType();
	};

	using AttributeOpt = std::optional<Attribute>;
	using AttributeList = std::vector<Attribute>;

}


#endif /* Attribute_hpp */
