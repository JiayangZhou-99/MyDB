#ifndef LOGDATABASESTATEMENT_HPP
#define LOGDATABASESTATEMENT_HPP

#include <string>
#include "Statement.hpp"

namespace MyDB {
	class LogUndoDataBaseStatement : public Statement {
	public:
		LogUndoDataBaseStatement(): Statement(StatementType::undoDataBase){}
	};

    class LogRedoDataBaseStatement : public Statement {
	public:
		LogRedoDataBaseStatement(): Statement(StatementType::redoDataBase){}
	};
}
#endif // !CreateDatabaseStatement