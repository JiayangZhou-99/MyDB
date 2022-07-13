#include "DatabaseManager.hpp"
#include "FolderReader.hpp"
#include "FolderView.hpp"
#include "DatabaseInstanceManager.hpp"
#include "QueryResultView.hpp"

namespace MyDB {
	void DatabaseManager::createDatabaseHandler(StatementPtr& aStatement)
	{
		CreateDatabaseStatement* theStatement = dynamic_cast<CreateDatabaseStatement*>(aStatement.get());

		if (DatabaseInstanceManager::databaseExists(theStatement->databaseName)) throw databaseExists;

		Database theDatabase(theStatement->databaseName, CreateDB());

		QueryResultView(1).show(output);
	}

	void DatabaseManager::dumpDatabaseHandler(StatementPtr& aStatement)
	{
		DumpDatabaseStatement* theStatement = dynamic_cast<DumpDatabaseStatement*>(aStatement.get());

		if (!DatabaseInstanceManager::databaseExists(theStatement->databaseName)) throw unknownDatabase;

		//We don't want to open the database twice
		if (DatabaseInstanceManager::hasActiveDatabase() &&
			DatabaseInstanceManager::getActiveDatabase()->getDatabaseName() == theStatement->databaseName)
		{
			DatabaseInstanceManager::getActiveDatabase()->dump(output);
		}
		else {
			Database(theStatement->databaseName, OpenDB()).dump(output);
		}
	}

	void DatabaseManager::dropDatabaseHandler(StatementPtr& aStatement)
	{
		DropDatabaseStatement* theStatement = dynamic_cast<DropDatabaseStatement*>(aStatement.get());

		if (!DatabaseInstanceManager::databaseExists(theStatement->databaseName)) throw unknownDatabase;

		//Asking to drop the database that is currently in use
		//so we need to release its resources
		if (DatabaseInstanceManager::hasActiveDatabase() &&
			DatabaseInstanceManager::getActiveDatabase()->getDatabaseName() == theStatement->databaseName)
		{
			DatabaseInstanceManager::closeActiveDatabase();
		}

		std::string thePath = Config::getDBPath(theStatement->databaseName);
		if (std::filesystem::remove(thePath))
		{
			QueryResultView(0).show(output);
		}
		else
		{
			throw deleteError;
		}
	}

	void DatabaseManager::showDatabasesHandler(StatementPtr& aStatement)
	{
		FolderView(Config::getStoragePath()).show(output);
	}

	void DatabaseManager::useDatabaseHandler(StatementPtr& aStatement)
	{

		UseDatabaseStatement* theStatement = dynamic_cast<UseDatabaseStatement*>(aStatement.get());
		
		if(DatabaseInstanceManager::hasActiveDatabase()){
			DatabaseInstanceManager::closeActiveDatabase();
		}

		DatabaseInstanceManager::setActiveDatabase(theStatement->databaseName);
		output << "Database changed\n";
	}

	void DatabaseManager::undoDatabaseHandler(StatementPtr& aStatement){

		// LogUndoDataBaseStatement* theStatement = dynamic_cast<LogUndoDataBaseStatement*>(aStatement.get());
		if (!DatabaseInstanceManager::hasActiveDatabase()) throw noDatabaseSpecified;
		DatabaseInstanceManager::getActiveDatabase()->undo();

	}

	void DatabaseManager::redoDatabaseHandler(StatementPtr& aStatement){

		// LogRedoDataBaseStatement* theStatement = dynamic_cast<LogRedoDataBaseStatement*>(aStatement.get());
		if (!DatabaseInstanceManager::hasActiveDatabase()) throw noDatabaseSpecified;
		DatabaseInstanceManager::getActiveDatabase()->redo();

	}

	void MyDB::DatabaseManager::run(StatementPtr& aStatement)
	{
		//Gets the function pointer from the map, dereferences it based
		//on the current object then pass parameters and return what the
		//called function returns
		(this->*dispatcher.at(aStatement->getType()))(aStatement);
	}

	ManagerPtr DatabaseManager::handles(StatementPtr& aStatement)
	{
		if (dispatcher.find(aStatement->getType()) != dispatcher.end())
		{
			return this;
		}
		return nullptr;
	}
}
