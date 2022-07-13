#ifndef DROPTABLESTATEMENT_HPP
#define DROPTABLESTATEMENT_HPP

#include <string>
#include "Statement.hpp"

namespace MyDB {
	class DropTableStatement : public Statement {
	public:
		DropTableStatement(std::string aTableName) : Statement(StatementType::dropTable), tableName(aTableName) { }

		const std::string tableName;
	};
}

#endif // !DROPTABLESTATEMENT_HPP
