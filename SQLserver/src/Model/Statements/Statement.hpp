//
//  Statement.hpp
//

#ifndef Statement_hpp
#define Statement_hpp

#include <memory>

namespace MyDB {
	class Statement;

	using StatementPtr = std::unique_ptr<Statement>;

	enum class StatementType {
		//Database Statements
		createDatabase,
		dropDatabase,
		showDatabases,
		useDatabase,
		dumpDatabase,
		undoDataBase,
		redoDataBase,

		//Table Statements
		createTable,
		dropTable,
		describeTable,
		showTables,
        insertTable,
        selectTable,
        deleteTable,
        updateTable,
        alterTable,
        indexing,

		//Application Statements
		version,
		quit,
		help,
        runScript,
	};

	class Statement {
	public:
		virtual ~Statement() = default;

		StatementType getType() const { return type; }

	protected:
		Statement(StatementType aType) : type(aType) {}

		StatementType   type;
	};

}

#endif /* Statement_hpp */
