#ifndef DATABASE_MANAGER_HPP
#define DATABASE_MANAGER_HPP

#include <iostream>
#include <map>
#include "Database.hpp"
#include "Statements.hpp"
#include "Manager.hpp"


namespace MyDB {

	class DatabaseManager : public Manager {
		using DatabaseManagerHandler = void(DatabaseManager::*)(StatementPtr&);

	public:
		DatabaseManager(std::ostream& anOutput) : Manager(anOutput) {}

		void run(StatementPtr& aStatement) override;

		ManagerPtr handles(StatementPtr& aStatement) override;

	protected:

		std::map<StatementType, DatabaseManagerHandler> dispatcher = {
			{StatementType::createDatabase, &DatabaseManager::createDatabaseHandler},
			{StatementType::dumpDatabase,   &DatabaseManager::dumpDatabaseHandler},
			{StatementType::dropDatabase,   &DatabaseManager::dropDatabaseHandler},
			{StatementType::showDatabases,  &DatabaseManager::showDatabasesHandler},
			{StatementType::useDatabase,    &DatabaseManager::useDatabaseHandler},
			{StatementType::redoDataBase,   &DatabaseManager::undoDatabaseHandler},
			{StatementType::undoDataBase,   &DatabaseManager::redoDatabaseHandler},
		};

		//Database Statements
		void createDatabaseHandler(StatementPtr& aStatement);
		void dumpDatabaseHandler(StatementPtr& aStatement);
		void dropDatabaseHandler(StatementPtr& aStatement);
		void showDatabasesHandler(StatementPtr& aStatement);
		void useDatabaseHandler(StatementPtr& aStatement);
		void undoDatabaseHandler(StatementPtr& aStatement);
		void redoDatabaseHandler(StatementPtr& aStatement);
		
	};

}
#endif
