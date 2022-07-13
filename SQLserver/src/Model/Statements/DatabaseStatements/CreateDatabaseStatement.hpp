#ifndef CREATEDATABASESTATEMENT_HPP
#define CREATEDATABASESTATEMENT_HPP

#include <string>
#include "Statement.hpp"

namespace MyDB {
	class CreateDatabaseStatement : public Statement {
	public:
		CreateDatabaseStatement(std::string aDatabaseName) : Statement(StatementType::createDatabase), databaseName(aDatabaseName) { }

		const std::string databaseName;
	};
}
#endif // !CreateDatabaseStatement
