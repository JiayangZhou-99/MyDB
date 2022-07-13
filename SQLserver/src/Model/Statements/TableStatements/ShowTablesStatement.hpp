#ifndef SHOWTABLESSTATEMENT_HPP
#define SHOWTABLESSTATEMENT_HPP

#include <string>
#include "Statement.hpp"

namespace MyDB {
	class ShowTablesStatement : public Statement {
	public:
		ShowTablesStatement() : Statement(StatementType::showTables) { }
	};
}

#endif // !SHOWTABLESSTATEMENT_HPP
