#ifndef DROPDATABASESTATEMENT_HPP
#define DROPDATABASESTATEMENT_HPP

#include <string>
#include "Statement.hpp"

namespace MyDB {
	class DropDatabaseStatement : public Statement {
	public:
		DropDatabaseStatement(std::string aDatabaseName) : Statement(StatementType::dropDatabase), databaseName(aDatabaseName) { }

		const std::string databaseName;
	};
}
#endif // !DropDatabaseStatement
