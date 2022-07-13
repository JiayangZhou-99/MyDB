#include "SQLManager.hpp"
#include "DatabaseInstanceManager.hpp"

namespace MyDB {

	void SQLManager::run(StatementPtr& aStatement)
	{
		//Gets the function pointer from the map, dereferences it based
		//on the current object then pass parameters and return what the
		//called function returns
		(this->*dispatcher.at(aStatement->getType()))(aStatement);
	}

	ManagerPtr SQLManager::handles(StatementPtr& aStatement)
	{
		if (dispatcher.find(aStatement->getType()) != dispatcher.end())
		{
			return this;
		}
		return nullptr;
	}

	void SQLManager::createTableHandler(StatementPtr& aStatement)
	{
		CreateTableStatement* theStatementPtr = dynamic_cast<CreateTableStatement*>(aStatement.get());

		if (!DatabaseInstanceManager::hasActiveDatabase()) throw noDatabaseSpecified;

		DatabaseInstanceManager::getActiveDatabase()->createTable(output, theStatementPtr->entity);
	}

	void SQLManager::dropTableHandler(StatementPtr& aStatement)
	{
		DropTableStatement* theStatementPtr = dynamic_cast<DropTableStatement*>(aStatement.get());

		if (!DatabaseInstanceManager::hasActiveDatabase()) throw noDatabaseSpecified;

		DatabaseInstanceManager::getActiveDatabase()->dropTable(output, theStatementPtr->tableName);
	}

	void SQLManager::describeTableHandler(StatementPtr& aStatement)
	{
		DescribeTableStatement* theStatementPtr = dynamic_cast<DescribeTableStatement*>(aStatement.get());

		if (!DatabaseInstanceManager::hasActiveDatabase()) throw noDatabaseSpecified;

		DatabaseInstanceManager::getActiveDatabase()->describeTable(output, theStatementPtr->tableName);
	}

	void SQLManager::showTablesHandler(StatementPtr& aStatement)
	{
		if (!DatabaseInstanceManager::hasActiveDatabase()) throw noDatabaseSpecified;

		DatabaseInstanceManager::getActiveDatabase()->showTables(output);
	}

	void SQLManager::insertHandler(StatementPtr& aStatement)
	{
		InsertStatement* theStatementPtr = dynamic_cast<InsertStatement*>(aStatement.get());

		if (!DatabaseInstanceManager::hasActiveDatabase()) throw noDatabaseSpecified;

		DatabaseInstanceManager::getActiveDatabase()->insert(output, theStatementPtr->rowData, theStatementPtr->tableName);
	}

	void SQLManager::selectHandler(StatementPtr& aStatement) {

		SelectStatement* theStatementPtr = dynamic_cast<SelectStatement*>(aStatement.get());

		if (!DatabaseInstanceManager::hasActiveDatabase()) throw noDatabaseSpecified;

		DatabaseInstanceManager::getActiveDatabase()->select(output, theStatementPtr->selectQueryPtr);

	}

	void SQLManager::updateHandler(StatementPtr& aStatement) {
		UpdateStatement* theStatementPtr = dynamic_cast<UpdateStatement*>(aStatement.get());

		if (!DatabaseInstanceManager::hasActiveDatabase()) throw noDatabaseSpecified;

		DatabaseInstanceManager::getActiveDatabase()->updateData(output, theStatementPtr->updateQueryPtr);
	}

	void SQLManager::deleteHandler(StatementPtr& aStatement) {
		DeleteStatement* theStatementPtr = dynamic_cast<DeleteStatement*>(aStatement.get());

		if (!DatabaseInstanceManager::hasActiveDatabase()) throw noDatabaseSpecified;

		DatabaseInstanceManager::getActiveDatabase()->deleteData(output, theStatementPtr->deleteQueryPtr);
	}


    void SQLManager::indexHandler(StatementPtr& aStatement){
        IndexStatement* theStatementPtr = dynamic_cast<IndexStatement*>(aStatement.get());

        if (!DatabaseInstanceManager::hasActiveDatabase()) throw noDatabaseSpecified;
        if(theStatementPtr->showIndexes){
            DatabaseInstanceManager::getActiveDatabase()->showIndexing(output);
        }else{
            DatabaseInstanceManager::getActiveDatabase()->getIndex(output, theStatementPtr->indexFields,theStatementPtr->entityName);
        }
    }


    void SQLManager::alterHandler(StatementPtr& aStatement){
        if (!DatabaseInstanceManager::hasActiveDatabase()) throw noDatabaseSpecified;
        AlterTableStatement* theStatementPtr = dynamic_cast<AlterTableStatement*>(aStatement.get());
        if(theStatementPtr->alterPtr->operation == "add"){
            DatabaseInstanceManager::getActiveDatabase()->alterEntityAddAttribute(output,theStatementPtr->alterPtr->tableName,theStatementPtr->alterPtr->attribute);
        }else{
            DatabaseInstanceManager::getActiveDatabase()->alterEntityDropAttribute(output,theStatementPtr->alterPtr->tableName,theStatementPtr->alterPtr->attriName);
        }
    }
}
