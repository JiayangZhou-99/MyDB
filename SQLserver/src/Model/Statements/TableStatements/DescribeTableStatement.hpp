#ifndef DESCRIBETABLESTATEMENT_HPP
#define DESCRIBETABLESTATEMENT_HPP

#include <string>
#include "Statement.hpp"

namespace MyDB {
	class DescribeTableStatement : public Statement {
	public:
		DescribeTableStatement(std::string aTableName) : Statement(StatementType::describeTable), tableName(aTableName) { }

		const std::string tableName;
	};
}

#endif // !DESCRIBETABLESTATEMENT_HPP
