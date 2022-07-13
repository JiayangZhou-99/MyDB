//
//  Row.hpp
//
//
//

#ifndef Row_hpp
#define Row_hpp

#include <string>
#include <vector>
#include <memory>
#include "BasicTypes.hpp"

//feel free to use this, or create your own version...

namespace MyDB {

	class Row {
	public:

		Row();

		~Row();

		Row& operator=(const Row& aRow);
		bool operator==(Row& aCopy) const;

		Row& set(const std::string& aKey,
			Value& aValue);

		KeyValues& getData() { return data; }
		void clearData() {
			data.clear();
		}

	protected:
		KeyValues data;
	};

	//-------------------------------------------

	using RowCollection = std::vector<Row>;
	using RowCollectionPtr = std::unique_ptr<RowCollection>;
}

#endif /* Row_hpp */
