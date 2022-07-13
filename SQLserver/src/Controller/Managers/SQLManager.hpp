#ifndef SQLMANAGER_HPP
#define SQLMANAGER_HPP

#include <iostream>
#include <map>
#include "Statements.hpp"
#include "Manager.hpp"

namespace MyDB {
	class SQLManager : public Manager {
		using SQLManagerHandler = void(SQLManager::*)(StatementPtr&);

	public:
		SQLManager(std::ostream& anOutput) : Manager(anOutput) {}

		void run(StatementPtr& aStatement) override;

		ManagerPtr handles(StatementPtr& aStatement) override;

	protected:
		std::map<StatementType, SQLManagerHandler> dispatcher = {
			{StatementType::createTable, &SQLManager::createTableHandler},
			{StatementType::dropTable, &SQLManager::dropTableHandler},
			{StatementType::describeTable, &SQLManager::describeTableHandler},
			{StatementType::showTables, &SQLManager::showTablesHandler},
			{StatementType::insertTable, &SQLManager::insertHandler},
			{StatementType::selectTable, &SQLManager::selectHandler},
			{StatementType::updateTable, &SQLManager::updateHandler},
			{StatementType::deleteTable, &SQLManager::deleteHandler},
            {StatementType::indexing, &SQLManager::indexHandler},
            {StatementType::alterTable, &SQLManager::alterHandler},
		};

		//Table Statements
		void createTableHandler(StatementPtr& aStatement);
		void dropTableHandler(StatementPtr& aStatement);
		void describeTableHandler(StatementPtr& aStatement);
		void showTablesHandler(StatementPtr& aStatement);
		void insertHandler(StatementPtr& aStatement);
		void selectHandler(StatementPtr& aStatement);
		void updateHandler(StatementPtr& aStatement);
		void deleteHandler(StatementPtr& aStatement);
        void indexHandler(StatementPtr& aStatement);
        void alterHandler(StatementPtr& aStatement);
	};
}

#endif // !SQLMANAGER_HPP
