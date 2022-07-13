#ifndef USEDATABASESTATEMENT_HPP
#define USEDATABASESTATEMENT_HPP

#include <string>
#include "Statement.hpp"

namespace MyDB {
	class UseDatabaseStatement : public Statement {
	public:
		UseDatabaseStatement(std::string aDatabaseName) : Statement(StatementType::useDatabase), databaseName(aDatabaseName) { }

		const std::string databaseName;
	};
}
#endif // !DropDatabaseStatement
