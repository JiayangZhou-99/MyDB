//
//  InsertStatement.hpp
//

#ifndef InsertTableStatement_hpp
#define InsertTableStatement_hpp

#include "Statement.hpp"
#include "Row.hpp"

namespace MyDB {
	class InsertStatement : public Statement {
	public:
		InsertStatement(RowCollectionPtr& aSetOfRows, std::string aTableName) :
			Statement(StatementType::insertTable), rowData(std::move(aSetOfRows)), tableName(aTableName) {}

		RowCollectionPtr rowData;
		const std::string tableName;
	};
}

#endif /* InsertTableStatement_hpp */
