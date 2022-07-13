#ifndef SHOWDATABASESSTATEMENT_HPP
#define SHOWDATABASESSTATEMENT_HPP

#include "Statement.hpp"

namespace MyDB {
	class ShowDatabasesStatement : public Statement {
	public:
		ShowDatabasesStatement() : Statement(StatementType::showDatabases) { }
	};
}
#endif // !ShowDatabasesStatement
