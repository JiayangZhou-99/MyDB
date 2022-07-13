#ifndef DUMPDATABASESTATEMENT_HPP
#define DUMPDATABASESTATEMENT_HPP

#include <string>
#include "Statement.hpp"

namespace MyDB {
	class DumpDatabaseStatement : public Statement {
	public:
		DumpDatabaseStatement(std::string aDatabaseName) : Statement(StatementType::dumpDatabase), databaseName(aDatabaseName) { }

		const std::string databaseName;
	};
}
#endif // !DumpDatabaseStatement
